#ifndef GRPC_ADAPTER_H
#define GRPC_ADAPTER_H

#include <future>

#include "adaptee.h"
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "manager.grpc.pb.h"

using namespace std;
using google::protobuf::Any;
using google::protobuf::Empty;
using google::protobuf::StringValue;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using zdautomotive::protobuf::SERVICE_TYPE;
using zdautomotive::protobuf::ZDRequest;
using zdautomotive::protobuf::ZDResponse;
using zdautomotive::protobuf::ZDServiceRequest;
using zdautomotive::protobuf::ZDServiceSubscriber;
using zdautomotive::protobuf::ZDServiceSubscriber_SUBSCRIBER_TYPE;
class GrpcWorker
{
public:
    GrpcWorker();
    virtual ~GrpcWorker();
    virtual void doWork() = 0;
    virtual void stopWork() = 0;
};
class GrpcAdapter : public GrpcWorker
{
public:
    GrpcAdapter(GrpcAdaptee *);
    ~GrpcAdapter();
    virtual void doWork();
    virtual void stopWork();

private:
    bool isWaitForWorkAgain;
    bool isClearInterval;
    GrpcAdaptee *adaptee;
    unique_ptr<zdautomotive::protobuf::ServiceManager::Stub> stub;
    void waitForWorkAgain();
    void sendPingReqToGrpcServer(promise<bool> &);
    void sendRegisterReqToGrpcServer(future<bool> &, promise<bool> &);
    void sendKeepAliveReqToGrpcServer(future<bool> &);
    void submitRequestResult(ZDServiceRequestResult &);
    void sendUnregisterReqToGrpcServer();
};

#endif