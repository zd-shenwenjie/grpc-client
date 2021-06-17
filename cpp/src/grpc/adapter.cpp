#include <iostream>
#include <thread>
#include <functional>
#include <exception>
#include <memory>

#include "adapter.h"
#include "../until/timer.h"
#include "../service/observer.h"
#include "../service/provider.h"

GrpcWorker::GrpcWorker()
{
    // cout << "GrpcWorker" << endl;
}

GrpcWorker::~GrpcWorker()
{
    // cout << "~GrpcWorker" << endl;
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
    isWaitForWorkAgain = false;
    promise<bool> promise_ping;
    promise<bool> promise_register;
    promise<bool> promise_keep_alive;
    future<bool> future_ping = promise_ping.get_future();
    future<bool> future_register = promise_register.get_future();
    future<bool> future_keep_alive = promise_keep_alive.get_future();
    thread t1(&GrpcAdapter::sendPingReqToGrpcServer, this, ref(promise_ping));
    thread t2(&GrpcAdapter::sendRegisterReqToGrpcServer, this, ref(future_ping), ref(promise_register));
    thread t3(&GrpcAdapter::sendKeepAliveReqToGrpcServer, this, ref(future_register), ref(promise_keep_alive));
    thread t4(&GrpcAdapter::waitForBindSubscriberCompleted, this, ref(future_keep_alive));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

void GrpcAdapter::stopWork()
{
    cout << "observer stop work" << endl;
}

void GrpcAdapter::waitForWorkAgain()
{
    if (!isWaitForWorkAgain)
    {
        isWaitForWorkAgain = true;
        cout << "wait For do Work again" << endl;
        Timer t;
        t.setTimeout([&]()
                     { doWork(); },
                     10 * 1000);
    }
}

void GrpcAdapter::sendPingReqToGrpcServer(promise<bool> &promise_ping)
{
    cout << "send ping req to grpc server" << endl;
    ClientContext context;
    Empty req;
    Empty res;
    Status status = stub->ping(&context, req, &res);
    if (!status.ok())
    {
        cout << "grpc server is offline." << endl;
        promise_ping.set_value(false);
        waitForWorkAgain();
    }
    else
    {
        cout << "grpc server is online." << endl;
        promise_ping.set_value(true);
    }
}

void GrpcAdapter::sendRegisterReqToGrpcServer(future<bool> &future_ping, promise<bool> &promise_register)
{
    bool is_ping_success = future_ping.get();
    if (!is_ping_success)
    {
        return;
    }
    cout << "send register req to grpc server" << endl;
    ClientContext context;
    ZDServiceSubscriber subscriber;
    string subscriberId = adaptee->getSubscriberId();
    // cout << "subscriberId:" << subscriberId << endl;
    subscriber.set_id(subscriberId);
    ZDServiceSubscriber_SUBSCRIBER_TYPE subscriberType = adaptee->getSubscriberType();
    // cout << "subscriberType:" << subscriberType << endl;
    subscriber.set_type(subscriberType);
    SERVICE_TYPE *p_service_type = adaptee->getSubscriberServiceTypes();
    int sub_service_num = adaptee->getSubscriberServiceTypesNum();
    for (int i = 0; i < sub_service_num; i++)
    {
        subscriber.add_servicetype(p_service_type[i]);
        // cout << "add_servicetype:" << p_service_type[i] << endl;
        p_service_type++;
    }
    ZDServiceRequest request;
    unique_ptr<ClientReader<ZDServiceRequest>> reader(stub->subscribe(&context, subscriber));
    promise_register.set_value(true);
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
    Status status = reader->Finish();
    if (!status.ok())
    {
        cout << "finish register stream failed." << endl;
        waitForWorkAgain();
    }
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

void GrpcAdapter::sendKeepAliveReqToGrpcServer(future<bool> &future_register, promise<bool> &promise_keep_alive)
{
    bool is_register_success = future_register.get();
    if (!is_register_success)
    {
        return;
    }
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
            // cout << "add_service:" << service_type << "," << service_status << endl;
        }
    }
    stream->Write(subscriber_status);
    // stream->WritesDone();
    promise_keep_alive.set_value(true);
    ZDResponse res;
    while (stream->Read(&res))
    {
        cout << "get response from keep alive stream ... "
             << "code:" << res.code() << ","
             << "message:" << res.message() << endl;
    }
    Status status = stream->Finish();
    if (!status.ok())
    {
        cout << "finish keep alive stream failed." << endl;
        waitForWorkAgain();
    }
}

void GrpcAdapter::waitForBindSubscriberCompleted(future<bool> &future_keep_alive)
{
    bool is_keep_alive_success = future_keep_alive.get();
    if (!is_keep_alive_success)
    {
        return;
    }
    cout << "wait grpc server bind subscriber completed." << endl;
    // Timer t = new Timer();
    // t.setInterval([]() { }, 10 * 1000);
    // t.clearInterval();
}