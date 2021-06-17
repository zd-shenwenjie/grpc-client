#include <iostream>
#include "adapter.h"
#include "../until/timer.h"
#include "../service/observer.h"
#include "../service/provider.h"

GrpcWorker::GrpcWorker()
{
    cout << "GrpcWorker" << endl;
}

GrpcWorker::~GrpcWorker()
{
    cout << "~GrpcWorker" << endl;
}

GrpcAdapter::GrpcAdapter(GrpcAdaptee *p)
{
    cout << "GrpcAdapter" << endl;
    adaptee = p;
    string host = p->getGrpcHost();
    if (host.empty())
    {
        host = "localhost:5000";
    }
    std::shared_ptr<Channel> channel = grpc::CreateChannel(host, grpc::InsecureChannelCredentials());
    stub = zdautomotive::protobuf::ServiceManager::NewStub(channel);
    isWaitForWorkAgain = false;
}

GrpcAdapter::~GrpcAdapter()
{
    cout << "~GrpcAdapter" << endl;
}

void GrpcAdapter::doWork()
{
    // sendPingReqToGrpcServer();
    // sendRegisterReqToGrpcServer();
    // sendKeepAliveReqToGrpcServer();
    // waitForBindSubscriberCompleted();
}

void GrpcAdapter::stopWork()
{
    cout << "observer stop work" << endl;
}

void GrpcAdapter::waitForWorkAgain()
{
    if (isWaitForWorkAgain)
    {
        cout << "wait For do Work again" << endl;
        Timer t;
        t.setTimeout([&]()
                     { doWork(); },
                     10 * 1000);
    }
}

void GrpcAdapter::sendPingReqToGrpcServer()
{
    cout << "send ping req to grpc server" << endl;
    ClientContext context;
    Empty req;
    Empty res;
    Status status = stub->ping(&context, req, &res);
    if (!status.ok())
    {
        cout << "grpc server is offline." << endl;
        waitForWorkAgain();
    }
    else
    {
        cout << "grpc server is online." << endl;
    }
}

void GrpcAdapter::sendRegisterReqToGrpcServer()
{
    cout << "send register req to grpc server" << endl;
    ClientContext context;
    ZDServiceSubscriber subscriber;
    string subscriberId = adaptee->getSubscriberId();
    cout << "subscriberId:" << subscriberId << endl;
    subscriber.set_id(subscriberId);
    ZDServiceSubscriber_SUBSCRIBER_TYPE subscriberType = adaptee->getSubscriberType();
    cout << "subscriberType:" << subscriberType << endl;
    subscriber.set_type(subscriberType);
    SERVICE_TYPE *p_service_type = adaptee->getSubscriberServiceTypes();
    int sub_service_num = adaptee->getSubscriberServiceTypesNum();
    for (int i = 0; i < sub_service_num; i++)
    {
        subscriber.add_servicetype(p_service_type[i]);
        cout << "add_servicetype:" << p_service_type[i] << endl;
        p_service_type++;
    }
    ZDServiceRequest request;
    unique_ptr<ClientReader<ZDServiceRequest>> reader(stub->subscribe(&context, subscriber));
    while (reader->Read(&request))
    {
        if (adaptee != NULL)
        {
            cout << "handle req by subscriber type:" << subscriberType << endl;
            if (subscriberType == ZDServiceSubscriber_SUBSCRIBER_TYPE::ZDServiceSubscriber_SUBSCRIBER_TYPE_PROVIDER)
            {
                ZDServiceRequestResult result = ((Provider *)adaptee)->onSubscriberServiceRequest(request);
                submitRequestResult(result);
            }
            else
            {
                ((Observer *)adaptee)->onSubscriberServiceRequest(request);
            }
        }
    }
    // Status status = reader->Finish();
    // if (status.ok())
    // {
    //     cout << "finish register stream succeeded." << endl;
    // }
}

void GrpcAdapter::submitRequestResult(ZDServiceRequestResult &result)
{
    ClientContext context;
    ZDResponse res;
    Status status = stub->submitRequestResult(&context, result, &res);
    if (!status.ok())
    {
        cout << "submit req grpc server failed." << endl;
    }
    else
    {
        cout << "submit req grpc server successed." << endl;
        cout << "code:" << res.code() << endl;
        cout << "message:" << res.message() << endl;
    }
}

void GrpcAdapter::sendKeepAliveReqToGrpcServer()
{
    cout << "send keep alive req to grpc server." << endl;
    ClientContext context;
    shared_ptr<ClientReaderWriter<ZDSubscriberStatus, ZDResponse>> stream(stub->keepAlive(&context));
    ZDSubscriberStatus subscriber_status;
    string subscriberId = adaptee->getSubscriberId();
    subscriber_status.set_id(subscriberId);
    ZDServiceSubscriber_SUBSCRIBER_TYPE subscriberType = adaptee->getSubscriberType();
    if (subscriberType == ZDServiceSubscriber_SUBSCRIBER_TYPE::ZDServiceSubscriber_SUBSCRIBER_TYPE_PROVIDER)
    {
        ZDService *p_service_status = ((Provider *)adaptee)->getCurrentServiceStatus();
        int sub_service_num = adaptee->getSubscriberServiceTypesNum();
        for (int i = 0; i < sub_service_num; i++)
        {
            ZDService *p_service = subscriber_status.add_service();
            SERVICE_TYPE service_type = p_service_status->type();
            ZDService_SERVICE_STATUS service_status = p_service_status->status();
            p_service->set_type(service_type);
            p_service->set_status(service_status);
            p_service_status++;
            cout << "add_service:" << service_type << "," << service_status << endl;
        }
    }
    stream->Write(subscriber_status);
    stream->WritesDone();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    ZDResponse res;
    while (stream->Read(&res))
    {
        cout << "get response from keep alive stream ... " << endl;
        cout << "code:" << res.code() << endl;
        cout << "message:" << res.message() << endl;
    }
    // Status status = stream->Finish();
    // if (!status.ok())
    // {
    //     cout << "finish keep alive stream failed." << endl;
    // }
}

void GrpcAdapter::waitForBindSubscriberCompleted()
{
    cout << "waitForBindSubscriberCompleted" << endl;
    // Timer t = new Timer();
    // t.setInterval([&]() { }, 10 * 1000);
    // t.clearInterval();
}