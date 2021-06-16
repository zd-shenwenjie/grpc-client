import { GrpcSubscriberAdaptee } from '../lib/grpc/adaptee';
import { Any } from 'google-protobuf/google/protobuf/any_pb';
import { SERVICE_TYPE, SERVICE_TYPEMap } from '../lib/proto/wrappers_pb';
import { ZDServiceRequest } from '../lib/proto/manager_pb'
import logger from '../lib/utils/logger';
import { BlockFilter } from '../lib/proto/networkif_pb';

export default class Observer extends GrpcSubscriberAdaptee {

    public getSubscriberId(): string {
        return 'observer:9999';
    }

    public getGrpcHost(): string {
        return 'localhost:5000';
    }

    public getSubscriberServiceTypes(): SERVICE_TYPEMap[keyof SERVICE_TYPEMap][] {
        return [
            SERVICE_TYPE.SERVICE_SET_NETWORK
        ];
    }

    public onSubscriberServiceRequest(req: ZDServiceRequest): void {
        if (req) {
            logger.info('receive service req :', req.getType(), req.getSession());
            const type: SERVICE_TYPEMap[keyof SERVICE_TYPEMap] = req.getType();
            switch (type) {
                case SERVICE_TYPE.SERVICE_SET_NETWORK:
                    this.onReceiveExampleServiceReq(req);
                    break;
            }
        }
    }

    private onReceiveExampleServiceReq(req: ZDServiceRequest) {
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
            }
        }
    }

}