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
    sendRegisterReqToGrpcServer();
    // sendKeepAliveReqToGrpcServer();
    // waitForBindSubscriberCompleted();
}

void GrpcAdapter::stopWork()
{
    cout << "observer stop work" << endl;
    
}

void GrpcAdapter::waitForWorkAgain()
{
    if(isWaitForWorkAgain) {
    cout << "wait For do Work again" << endl;
    Timer t;
    t.setTimeout([&]() { 
        doWork();
    }, 10 * 1000);

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
    subscriber.set_id(subscriberId);
    // ZDServiceSubscriber_SUBSCRIBER_TYPE subscriberType = adaptee->getSubscriberType();
    // ZDServiceSubscriber_SUBSCRIBER_TYPE::ZDServiceSubscriber_SUBSCRIBER_TYPE_PROVIDER
    // subscriber.set_type(subscriberType);
    ZDServiceRequest request;
    unique_ptr<ClientReader<ZDServiceRequest>> reader(stub->subscribe(&context, subscriber));
    while (reader->Read(&request)) {
    //   std::cout << "Found feature called " << feature.name() << " at "
    //             << feature.location().latitude() / kCoordFactor_ << ", "
    //             << feature.location().longitude() / kCoordFactor_ << std::endl;
    }
    Status status = reader->Finish();
    if (status.ok()) {
      cout << "send register req succeeded." << endl;
    } else {
      cout << "send register req failed." << endl;
    }
    // if (adaptee != NULL)
    // {
    //     cout << adaptee->getSubscriberType() << endl;
    //     cout << adaptee->getSubscriberId() << endl;
    //     int subscriberType = adaptee->getSubscriberType();
    //     if (subscriberType)
    //     {
    //         string str = ((Provider *)adaptee)->onSubscriberServiceRequest("some req2");
    //         cout << str << endl;
    //     }
    //     else
    //     {
    //         ((Observer *)adaptee)->onSubscriberServiceRequest("some req1");
    //     }
    // }
}

void GrpcAdapter::sendKeepAliveReqToGrpcServer()
{
    cout << "sendKeepAliveReqToGrpcServer" << endl;
    // Timer t = new Timer();
    // t.setInterval([&]() {
    //     cout << "Hey.. helloworld" << endl;
    // }, 1000);
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    // t.clearInterval();
    // cout << "Hey.." << endl;
}

void GrpcAdapter::waitForBindSubscriberCompleted()
{
    cout << "waitForBindSubscriberCompleted" << endl;
}