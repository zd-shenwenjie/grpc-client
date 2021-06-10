import GrpcWorker from './grpcWorker';
import { GrpcSubscriberAdapter } from './grpcAdapter';
import { Any } from 'google-protobuf/google/protobuf/any_pb';
import { SERVICE_TYPE, SERVICE_TYPEMap } from '../proto/wrappers_pb';
import { ZDServiceRequest } from '../proto/management_pb'
import { YourRequestParameter } from '../proto/yourService_pb'
import logger from '../utils/logger';

class ObserverAdapter extends GrpcSubscriberAdapter {

    public getSubscriberId(): string {
        return 'observer:9999';
    }

    public getGrpcHost(): string {
        return 'localhost:5000';
    }

    public getSubscriberServiceTypes(): SERVICE_TYPEMap[keyof SERVICE_TYPEMap][] {
        return [
            SERVICE_TYPE.SERVICE_EXAMPLE_A,
            SERVICE_TYPE.SERVICE_EXAMPLE_B,
            SERVICE_TYPE.SERVICE_EXAMPLE_C
        ];
    }

    public onSubscriberServiceRequest(req: ZDServiceRequest): void {
        if (req) {
            const type: SERVICE_TYPEMap[keyof SERVICE_TYPEMap] = req.getType();
            switch (type) {
                case SERVICE_TYPE.SERVICE_EXAMPLE_A:
                case SERVICE_TYPE.SERVICE_EXAMPLE_B:
                case SERVICE_TYPE.SERVICE_EXAMPLE_C:
                    this.onReceiveExampleServiceReq(req);
                    break;
            }
        }
    }

    private onReceiveExampleServiceReq(req: ZDServiceRequest) {
        logger.info('receive req info:', req.getCreatetime(), req.getExpiretime(), req.getSession());
        const data: Any | undefined = req.getData();
        if (data) {
            const parameter = data.unpack(YourRequestParameter.deserializeBinary, 'zdautomotive.protobuf.YourRequestParameter');
            if (parameter) {
                logger.info(JSON.stringify(parameter.toString()));
            }
        }
    }

}

const woker = new GrpcWorker(new ObserverAdapter());
woker.doWork();

process.on('uncaughtException', (error: Error) => {
    logger.info(`uncaughtException: ${error.message}`);
    woker.stopWork();
});