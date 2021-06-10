/* eslint-disable max-classes-per-file */
/* tslint:disable max-classes-per-file */
import { ZDServiceRequest, ZDServiceSubscriber, ZDService } from '../proto/management_pb';
import { ZDServiceRequestResult } from '../proto/management_pb';
import { SERVICE_TYPEMap } from '../proto/wrappers_pb';

export abstract class GrpcSubscriberAdapter {
    abstract getSubscriberId(): string;
    abstract getGrpcHost(): string;
    abstract getSubscriberServiceTypes(): SERVICE_TYPEMap[keyof SERVICE_TYPEMap][];
    abstract onSubscriberServiceRequest(req: ZDServiceRequest): void;
    public subscriberType: ZDServiceSubscriber.SUBSCRIBER_TYPEMap[keyof ZDServiceSubscriber.SUBSCRIBER_TYPEMap];
    constructor() {
        this.subscriberType = ZDServiceSubscriber.SUBSCRIBER_TYPE.OBSERVER;
    }
}

export abstract class GrpcProviderAdapter extends GrpcSubscriberAdapter {
    abstract onSubscriberServiceRequest(req: ZDServiceRequest): ZDServiceRequestResult | null;
    abstract getCurrentServiceStatus(): ZDService[];
    constructor() {
        super();
        this.subscriberType = ZDServiceSubscriber.SUBSCRIBER_TYPE.PROVIDER;
    }
}