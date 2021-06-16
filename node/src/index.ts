import * as grpc from '@grpc/grpc-js';
import Provider from './service/provider';
import Observer from './service/observer';
import { GrpcWorker, GrpcAdapter } from './lib/grpc/adapter';
import { NetworkManagerClient } from './lib/proto/networkif_grpc_pb';
import { Any } from 'google-protobuf/google/protobuf/any_pb';
import { SERVICE_TYPE, ZDRequest, ZDResponse } from './lib/proto/wrappers_pb';
import { ProtocolTag, BlockFilter } from './lib/proto/networkif_pb';
import logger from './lib/utils/logger';

const mode = process.argv[process.argv.length - 1];
switch (mode) {
    case 'consumer': {
        // Use grpc-client as a consumer
        const client = new NetworkManagerClient('0.0.0.0:5000', grpc.credentials.createInsecure());
        const req = new ZDRequest();
        const data: Any = new Any();
        const filter: BlockFilter = new BlockFilter();
        filter.setPhyid(123);
        filter.setVlanid(123);
        filter.setSrcip('123');
        filter.setDstip('123');
        filter.setSrcport(123);
        filter.setDstport(123);
        data.pack(filter.serializeBinary(), 'zdautomotive.protobuf.BlockFilter');
        req.setData(data);
        logger.info(`send req(type:${SERVICE_TYPE.SERVICE_SET_NETWORK}) to grpc server.`)
        client.setNetwork(req, (err: grpc.ServiceError | null, res: ZDResponse | undefined) => {
            if (!err && res) {
                const code: ZDResponse.ERROR_CODEMap[keyof ZDResponse.ERROR_CODEMap] = res.getCode();
                const message: string = res.getMessage();
                logger.info('code:', code, 'message:', message);
                const anyData: Any | undefined = res.getData();
                if (anyData) {
                    const result: ProtocolTag | null = anyData.unpack(ProtocolTag.deserializeBinary, 'zdautomotive.protobuf.ProtocolTag');
                    if (result) {
                        logger.info('data:', result.getVlanid(), result.getSrcport(), result.getDstport(), result.getTag());
                    } else {
                        logger.info('unpack res any data is null.');
                    }
                } else {
                    logger.info('res data is null.');
                }
            } else {
                logger.info('send req error:', err?.message);
            }
        })
        break;
    }
    case 'provider': {
        // Use grpc-client as a provider
        const provider: Provider = new Provider();
        const woker: GrpcWorker = new GrpcAdapter(provider);
        woker.doWork();
        // woker.stopWork();
        break;
    }
    case 'observer': {
        // Use grpc-client as a observer
        const observer: Observer = new Observer();
        const woker: GrpcWorker = new GrpcAdapter(observer);
        woker.doWork();
        // woker.stopWork();
        break;
    }
}







