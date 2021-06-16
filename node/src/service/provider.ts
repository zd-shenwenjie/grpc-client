import { GrpcProviderAdaptee } from '../lib/grpc/adaptee';
import { Any } from 'google-protobuf/google/protobuf/any_pb';
import { SERVICE_TYPE, SERVICE_TYPEMap, ZDResponse } from '../lib/proto/wrappers_pb';
import { ZDServiceRequest, ZDServiceRequestResult, ZDService } from '../lib/proto/manager_pb'
import { BlockFilter, ProtocolTag } from '../lib/proto/networkif_pb';
import logger from '../lib/utils/logger';

export default class Provider extends GrpcProviderAdaptee {

    public getSubscriberId(): string {
        return 'provider:9999'
    }

    public getGrpcHost(): string {
        return 'localhost:5000';
    }

    public getSubscriberServiceTypes(): SERVICE_TYPEMap[keyof SERVICE_TYPEMap][] {
        return [
            SERVICE_TYPE.SERVICE_SET_NETWORK
        ];
    }

    public getCurrentServiceStatus(): ZDService[] {
        return [
            this.newZDService(SERVICE_TYPE.SERVICE_SET_NETWORK, ZDService.SERVICE_STATUS.ENABLED)
        ]
    }

    public onSubscriberServiceRequest(req: ZDServiceRequest): ZDServiceRequestResult | null {
        let result = null;
        const session: string = req.getSession();
        logger.info('receive service req :', req.getType(), req.getSession());
        if (req && session) {
            const type: SERVICE_TYPEMap[keyof SERVICE_TYPEMap] = req.getType();
            switch (type) {
                case SERVICE_TYPE.SERVICE_SYS_CANCEL:
                    logger.info(`req:${req.getSession()} timeout.`);
                    break;
                case SERVICE_TYPE.SERVICE_SET_NETWORK:
                    result = this.onHandleExampleServiceReq(req);
                    break;
            }
        }
        return result;
    }

    private onHandleExampleServiceReq(req: ZDServiceRequest): ZDServiceRequestResult | null {
        const data: Any | undefined = req.getData();
        if (data) {
            const args = data.unpack(BlockFilter.deserializeBinary, 'zdautomotive.protobuf.BlockFilter');
            if (args) {
                logger.info('unpack req args data:',
                    args.getPhyid(),
                    args.getVlanid(),
                    args.getSrcip(),
                    args.getDstip(),
                    args.getSrcport(),
                    args.getDstport());
                const result: ZDServiceRequestResult = new ZDServiceRequestResult();
                const response: ZDResponse = new ZDResponse();
                response.setCode(ZDResponse.ERROR_CODE.OK);
                response.setMessage('send req success.');
                const anyData: Any = new Any();
                const parameter: ProtocolTag = new ProtocolTag();
                parameter.setVlanid(456);
                parameter.setSrcport(456);
                parameter.setDstport(456);
                parameter.setTag('456');
                anyData.pack(parameter.serializeBinary(), 'zdautomotive.protobuf.ProtocolTag');
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