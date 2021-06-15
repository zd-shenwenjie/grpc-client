#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "networkif.grpc.pb.h"
// #include "./grpc/adapter.h"
// #include "./grpc/adaptee.h"
// #include "./service/observer.h"
// #include "./service/provider.h"

using namespace std;
using google::protobuf::Any;
using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using zdautomotive::protobuf::BlockFilter;
using zdautomotive::protobuf::ProtocolTag;
using zdautomotive::protobuf::SERVICE_TYPE;
using zdautomotive::protobuf::ZDRequest;
using zdautomotive::protobuf::ZDResponse;

// bool test(const BlockFilter &filter, ProtocolTag *tag)
// {
//     std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
//     std::unique_ptr<zdautomotive::protobuf::NetworkManager::Stub> stub_ = zdautomotive::protobuf::NetworkManager::NewStub(channel);
//     ClientContext context;
//     Status status = stub_->test(&context, filter, tag);
//     if (!status.ok())
//     {
//         cout << "test rpc failed." << endl;
//         return false;
//     }
//     cout << "vlanid:" << tag->vlanid() << endl;
//     return true;
// }

int main()
{
    cout << "Hello, world!" << endl;
    std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:5000", grpc::InsecureChannelCredentials());
    std::unique_ptr<zdautomotive::protobuf::NetworkManager::Stub> stub_ = zdautomotive::protobuf::NetworkManager::NewStub(channel);

    BlockFilter filter;
    ProtocolTag tag;
    filter.set_phyid(123);
    filter.set_vlanid(123);
    filter.set_srcip("123");
    filter.set_dstip("123");
    filter.set_srcport(123);
    filter.set_dstport(123);
    ClientContext context;
    // Status status = stub_->test(&context, filter, &tag);
    // if (!status.ok())
    // {
    //     cout << "test rpc failed." << endl;
    //     return false;
    // }
    // else
    // {
    //     cout << "vlanid:" << tag.vlanid() << endl;
    // }
    ZDRequest req;
    ZDResponse res;
    req.set_type(SERVICE_TYPE::SERVICE_EXAMPLE_A);
    Any any;
    any.PackFrom(filter);
    req.mutable_data()->MergeFrom(any);

    Status status = stub_->send(&context, req, &res);
    if (!status.ok())
    {
        cout << "send rpc failed." << endl;
        return false;
    }
    else
    {
        cout << "code:" << res.code() << endl;
        cout << "message:" << res.message() << endl;
        Any *p = res.mutable_data();
        Any anyData = *p;
        if (anyData.UnpackTo(&tag))
        {
            cout << "vlanid:" << tag.vlanid() << "dstport:" << tag.dstport() << "srcport:" << tag.srcport() << "tag:" << tag.tag() << endl;
        }
    }

    // GrpcAdaptee *observer = new Observer();
    // GrpcAdaptee *provider = new Provider();
    // GrpcWorker *worker = new GrpcAdapter(observer);
    // GrpcWorker *worker = new GrpcAdapter(provider);
    // worker->doWork();
    // worker->stopWork();
    // delete worker;
    // delete observer;
    // delete provider;
    return 0;
}