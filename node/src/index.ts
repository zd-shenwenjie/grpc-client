import * as grpc from '@grpc/grpc-js';
import Provider from './service/provider';
import Observer from './service/observer';
import { GrpcWorker, GrpcAdapter } from './lib/grpc/adapter';
import { YourServiceManagerClient } from './lib/proto/yourService_grpc_pb';
import { Any } from 'google-protobuf/google/protobuf/any_pb';
import { SERVICE_TYPE, ZDRequest, ZDResponse } from './lib/proto/wrappers_pb';
import { YourRequestParameter, YourResponseParameter } from './lib/proto/yourService_pb';
import logger from './lib/utils/logger';

const mode = process.argv[process.argv.length - 1];
switch (mode) {
    case 'consumer': {
        // Use grpc-client as a consumer
        const client = new YourServiceManagerClient('0.0.0.0:5000', grpc.credentials.createInsecure());
        const req = new ZDRequest();
        const data: Any = new Any();
        const args: YourRequestParameter = new YourRequestParameter();
        args.setNumber(123);
        data.pack(args.serializeBinary(), 'zdautomotive.protobuf.YourRequestParameter');
        req.setData(data);
        req.setType(SERVICE_TYPE.SERVICE_EXAMPLE_A);
        client.yourServiceRequestMethodA(req, (err, res) => {
            if (!err && res) {
                const code: ZDResponse.ERROR_CODEMap[keyof ZDResponse.ERROR_CODEMap] = res.getCode();
                const message: string = res.getMessage();
                logger.info('code:', code, 'message:', message);
                const anyData: Any | undefined = res.getData();
                if (anyData) {
                    const result: YourResponseParameter | null = anyData.unpack(YourResponseParameter.deserializeBinary, 'zdautomotive.protobuf.YourResponseParameter');
                    if (result) {
                        logger.info('data:', result.toString());
                    }
                }
            } else {
                logger.info('error', err?.message);
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







