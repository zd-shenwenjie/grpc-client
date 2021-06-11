import GrpcWorker from '../grpc/worker';
import { GrpcProviderAdapter } from '../grpc/adapter';
import { Any } from 'google-protobuf/google/protobuf/any_pb';
import { SERVICE_TYPE, SERVICE_TYPEMap, ZDResponse } from '../proto/wrappers_pb';
import { ZDServiceRequest, ZDServiceRequestResult, ZDService } from '../proto/management_pb'
import { YourRequestParameter, YourResponseParameter } from '../proto/yourService_pb'
import logger from '../utils/logger';

class ProviderAdapter extends GrpcProviderAdapter {

    public getSubscriberId(): string {
        return 'provider:9999'
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

    public getCurrentServiceStatus(): ZDService[] {
        return [
            this.newZDService(SERVICE_TYPE.SERVICE_EXAMPLE_A, ZDService.SERVICE_STATUS.ENABLED),
            this.newZDService(SERVICE_TYPE.SERVICE_EXAMPLE_B, ZDService.SERVICE_STATUS.ENABLED),
            this.newZDService(SERVICE_TYPE.SERVICE_EXAMPLE_C, ZDService.SERVICE_STATUS.ENABLED)
        ]
    }

    public onSubscriberServiceRequest(req: ZDServiceRequest): ZDServiceRequestResult | null {
        let result = null;
        const session: string = req.getSession();
        if (req && session) {
            const type: SERVICE_TYPEMap[keyof SERVICE_TYPEMap] = req.getType();
            switch (type) {
                case SERVICE_TYPE.SERVICE_SYS_CANCEL:
                    logger.info(`req:${req.getSession()} timeout.`);
                    break;
                case SERVICE_TYPE.SERVICE_EXAMPLE_A:
                case SERVICE_TYPE.SERVICE_EXAMPLE_B:
                case SERVICE_TYPE.SERVICE_EXAMPLE_C:
                    result = this.onHandleExampleServiceReq(req);
                    break;
            }
        }
        return result;
    }

    private onHandleExampleServiceReq(req: ZDServiceRequest): ZDServiceRequestResult | null {
        logger.info('receive req info:', req.getCreatetime(), req.getExpiretime(), req.getSession());
        const data: Any | undefined = req.getData();
        if (data) {
            const args = data.unpack(YourRequestParameter.deserializeBinary, 'zdautomotive.protobuf.YourRequestParameter');
            if (args) {
                logger.info(JSON.stringify(args.toString()));
                const result: ZDServiceRequestResult = new ZDServiceRequestResult();
                const response: ZDResponse = new ZDResponse();
                response.setCode(ZDResponse.ERROR_CODE.OK);
                response.setMessage('example req success.');
                const anyData: Any = new Any();
                const parameter: YourResponseParameter = new YourResponseParameter();
                parameter.setMessage('hello example!');
                anyData.pack(parameter.serializeBinary(), 'zdautomotive.protobuf.YourResponseParameter');
                req.setData(data);
                response.setData(anyData);
                result.setResponse(response);
                result.setRequest(req);
                return result;
            }
        }
        return null;
    }


    private newZDService(type: SERVICE_TYPEMap[keyof SERVICE_TYPEMap], status: ZDService.SERVICE_STATUSMap[keyof ZDService.SERVICE_STATUSMap] = ZDService.SERVICE_STATUS.ENABLED): ZDService {
        const service: ZDService = new ZDService();
        service.setType(type);
        service.setStatus(status);
        return service;
    }

}

const woker = new GrpcWorker(new ProviderAdapter());
woker.doWork();

process.on('uncaughtException', (error: Error) => {
    logger.info(`uncaughtException: ${error.message}`);
    woker.stopWork();
});