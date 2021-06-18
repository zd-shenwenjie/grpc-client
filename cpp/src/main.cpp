#include <iostream>
#include <string>

#include "./grpc/adapter.h"
#include "./grpc/adaptee.h"
#include "./service/observer.h"
#include "./service/provider.h"

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "networkif.grpc.pb.h"

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
using zdautomotive::protobuf::ZDRequest;
using zdautomotive::protobuf::ZDResponse;

void setNetwork(const ZDRequest &req, ZDResponse *res)
{
    std::shared_ptr<Channel> channel = grpc::CreateChannel("localhost:5000", grpc::InsecureChannelCredentials());
    std::unique_ptr<zdautomotive::protobuf::NetworkManager::Stub> stub = zdautomotive::protobuf::NetworkManager::NewStub(channel);
    ClientContext context;
    Status status = stub->setNetwork(&context, req, res);
    if (!status.ok())
    {
        cout << "send rpc failed." << endl;
        return;
    }
    else
    {
        cout << "code:" << res->code() << endl;
        cout << "message:" << res->message() << endl;
        Any *p = res->mutable_data();
        Any anyData = *p;
        ProtocolTag tag;
        if (anyData.UnpackTo(&tag))
        {
            cout << "vlanid:" << tag.vlanid() << ",dstport:" << tag.dstport() << ",srcport:" << tag.srcport() << ",tag:" << tag.tag() << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc >= 2)
    {
        string role(argv[1]);
        if (role == "-consumer")
        {
            ZDRequest req;
            BlockFilter filter;
            filter.set_phyid(123);
            filter.set_vlanid(123);
            filter.set_srcip("123");
            filter.set_dstip("123");
            filter.set_srcport(123);
            filter.set_dstport(123);
            Any any;
            any.PackFrom(filter);
            req.mutable_data()->MergeFrom(any);
            ZDResponse res;
            setNetwork(req, &res);
        }
        else if (role == "-provider")
        {
            GrpcAdaptee *provider = new Provider();
            GrpcWorker *worker = new GrpcAdapter(provider);
            worker->doWork();
            // worker->stopWork();
            // delete worker;
            // delete provider;
        }
        else if (role == "-observer")
        {
            GrpcAdaptee *observer = new Observer();
            GrpcWorker *worker = new GrpcAdapter(observer);
            worker->doWork();
            // worker->stopWork();
            // delete worker;
            // delete observer;
        }
    }
    cout << "hello world" << endl;
    while (getchar() != '\n')
        ;
    return 0;
}