import { ZDServiceRequest, ZDServiceSubscriber, ZDService, ZDServiceRequestResult } from '../proto/manager_pb';
import { SERVICE_TYPEMap } from '../proto/wrappers_pb';

abstract class GrpcAdaptee {
    abstract getSubscriberId(): string;
    abstract getGrpcHost(): string;
    abstract getSubscriberServiceTypes(): SERVICE_TYPEMap[keyof SERVICE_TYPEMap][];
}

export abstract class GrpcSubscriberAdaptee extends GrpcAdaptee {
    abstract onSubscriberServiceRequest(req: ZDServiceRequest): void;
    public subscriberType: ZDServiceSubscriber.SUBSCRIBER_TYPEMap[keyof ZDServiceSubscriber.SUBSCRIBER_TYPEMap];
    constructor() {
        super();
        this.subscriberType = ZDServiceSubscriber.SUBSCRIBER_TYPE.OBSERVER;
    }
}

export abstract class GrpcProviderAdaptee extends GrpcAdaptee {
    abstract onSubscriberServiceRequest(req: ZDServiceRequest): ZDServiceRequestResult | null;
    abstract getCurrentServiceStatus(): ZDService[];
    public subscriberType: ZDServiceSubscriber.SUBSCRIBER_TYPEMap[keyof ZDServiceSubscriber.SUBSCRIBER_TYPEMap];
    constructor() {
        super();
        this.subscriberType = ZDServiceSubscriber.SUBSCRIBER_TYPE.PROVIDER;
    }
}