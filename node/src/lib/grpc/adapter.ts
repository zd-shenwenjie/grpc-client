import * as grpc from '@grpc/grpc-js';
import { Empty } from 'google-protobuf/google/protobuf/empty_pb';
import { StringValue } from 'google-protobuf/google/protobuf/wrappers_pb';
import { ZDServiceSubscriber, ZDSubscriberStatus, ZDServiceRequestResult } from '../proto/manager_pb';
import { ServiceManagerClient } from '../proto/manager_grpc_pb';
import { ZDResponse } from '../proto/wrappers_pb';
import { GrpcProviderAdaptee, GrpcSubscriberAdaptee } from './adaptee';
import moment from 'moment';
import logger from '../utils/logger';

const TIME_KEEP_INTERAL = 30 * 1000;
const TIME_RECONNECT_INTERAL = 10 * 1000;
const TIME_DELAY_BIND = 15 * 1000;

export abstract class GrpcWorker {
    abstract doWork(): void;
    abstract stopWork(): void;
}

export class GrpcAdapter extends GrpcWorker {

    private adaptee: GrpcProviderAdaptee | GrpcSubscriberAdaptee;
    private client: ServiceManagerClient;
    private keepAliveStream: grpc.ClientDuplexStream<ZDSubscriberStatus, ZDResponse> | undefined = undefined;
    private keepAliveHandler: NodeJS.Timeout | null = null;
    private isConnecting = false;

    constructor(adaptee: GrpcProviderAdaptee | GrpcSubscriberAdaptee) {
        super();
        this.adaptee = adaptee;
        const host: string = this.adaptee.getGrpcHost();
        this.client = new ServiceManagerClient(host, grpc.credentials.createInsecure());
    }

    public doWork(): void {
        if (this.isConnecting === false) {
            this.isConnecting = true;
            this.sendPingReqToGrpcServer()
                .then(() => {
                    return this.sendRegisterReqToGrpcServer();
                }).then(() => {
                    return this.sendKeepAliveReqToGrpcServer();
                }).then((stream) => {
                    this.waitForBindSubscriberCompleted(stream);
                }).catch((error: Error) => {
                    logger.debug('doWork err:' + error.message + moment().format('MMMM Do YYYY, h:mm:ss a'));
                    this.waitForDoWorkAgain();
                })
        }
    }

    public stopWork(): void {
        if (this.keepAliveHandler) {
            clearInterval(this.keepAliveHandler);
            this.keepAliveHandler = null;
        }
        if (this.client) {
            this.client.unsubscribe(new StringValue().setValue(this.adaptee.getSubscriberId()), (err, res) => {
                if (!err && res) {
                    logger.info(res.getMessage());
                }
            })
        }
    }

    public sendSubscriberStatusToGrpcServer(): void {
        if (this.keepAliveStream) {
            if (this.isGrpcProviderAdapter(this.adaptee)) {
                const status: ZDSubscriberStatus = new ZDSubscriberStatus();
                const servicesList = this.adaptee.getCurrentServiceStatus();
                status.setId(this.adaptee.getSubscriberId());
                status.setServiceList(servicesList);
                this.keepAliveStream.write(status);
                logger.debug(`send provider: ${this.adaptee.getSubscriberId()} status ${moment().format('MMMM Do YYYY, h:mm:ss a')}.`);
            } else {
                const status: ZDSubscriberStatus = new ZDSubscriberStatus();
                status.setId(this.adaptee.getSubscriberId());
                this.keepAliveStream.write(status);
                logger.debug(`send subscriber: ${this.adaptee.getSubscriberId()} status ${moment().format('MMMM Do YYYY, h:mm:ss a')}.`);
            }
        }
    }

    private waitForDoWorkAgain(): void {
        this.isConnecting = false;
        if (this.keepAliveHandler !== null) {
            clearInterval(this.keepAliveHandler);
            this.keepAliveHandler = null;
            logger.info('clear keep alive timer.');
        }
        setTimeout(() => {
            this.doWork();
        }, TIME_RECONNECT_INTERAL);
    }

    private onReceiveGrpcServerKeepAliveReply(): void {
        this.isConnecting = false;
        if (this.keepAliveHandler === null) {
            // logger.info('new a keep alive timer.');
            this.keepAliveHandler = setInterval(() => {
                this.sendSubscriberStatusToGrpcServer();
            }, TIME_KEEP_INTERAL);
        }
        logger.info('receive keep alive reply');
    }

    private sendPingReqToGrpcServer(): Promise<void> {
        logger.debug(`send ping req to ${this.adaptee.getGrpcHost()}`);
        return new Promise<void>((resolve, reject) => {
            this.client.ping(new Empty(), (error) => {
                if (error) {
                    reject(new Error('grpc server is offline.'));
                } else {
                    logger.debug('grpc server is online.');
                    resolve();
                }
            });
        })
    }

    private sendRegisterReqToGrpcServer(): Promise<void> {
        logger.debug(`send register req to ${this.adaptee.getGrpcHost()}`);
        return new Promise<void>((resolve, reject) => {
            if (this.client) {
                const sub = new ZDServiceSubscriber();
                sub.setId(this.adaptee.getSubscriberId());
                sub.setType(this.adaptee.subscriberType);
                sub.setServicetypeList(this.adaptee.getSubscriberServiceTypes());
                const stream = this.client.subscribe(sub);
                stream.on('data', (req) => {
                    if (this.isGrpcProviderAdapter(this.adaptee)) {
                        const result: ZDServiceRequestResult | null = this.adaptee.onSubscriberServiceRequest(req);
                        if (result !== null) {
                            this.client.submitRequestResult(result, (err, res?) => {
                                if (!err && res) {
                                    logger.info('submit req result:' + res.getMessage());
                                }
                            });
                        }
                    } else {
                        this.adaptee.onSubscriberServiceRequest(req);
                    }
                });
                resolve();
            } else {
                reject(new Error('register error.'));
            }
        })
    }

    private sendKeepAliveReqToGrpcServer(): Promise<grpc.ClientDuplexStream<ZDSubscriberStatus, ZDResponse>> {
        logger.debug('send keep alive req to grpc server.');
        return new Promise<grpc.ClientDuplexStream<ZDSubscriberStatus, ZDResponse>>((resolve, reject) => {
            if (this.client) {
                const stream = this.client.keepAlive();
                if (stream) {
                    this.keepAliveStream = stream;
                    if (this.keepAliveHandler === null) {
                        setTimeout(() => {
                            this.sendSubscriberStatusToGrpcServer();
                        }, TIME_DELAY_BIND);
                    }
                    resolve(stream);
                } else {
                    reject(new Error('keep alive error.'));
                }
            }
        })
    }

    private waitForBindSubscriberCompleted(stream: grpc.ClientDuplexStream<ZDSubscriberStatus, ZDResponse>): void {
        logger.debug('wait grpc server bind subscriber completed.');
        stream.on('data', (response: ZDResponse) => {
            logger.info('recive pong from grpc server', response.getCode());
            if (response.getCode() === ZDResponse.ERROR_CODE.OK) {
                this.onReceiveGrpcServerKeepAliveReply();
            } else {
                logger.debug('bind error:' + response.getMessage());
                this.waitForDoWorkAgain();
            }
        });
        stream.on('error', (error) => {
            logger.debug('grpc client keep alive stream error.', error.message);
            this.waitForDoWorkAgain();
        });
        stream.on('end', () => {
            logger.debug('grpc client keep alive stream end.');
            this.waitForDoWorkAgain();
        });
    }

    private isGrpcProviderAdapter(adapter: GrpcProviderAdaptee | GrpcSubscriberAdaptee): adapter is GrpcProviderAdaptee {
        return adapter.subscriberType === ZDServiceSubscriber.SUBSCRIBER_TYPE.PROVIDER;
    }
}



