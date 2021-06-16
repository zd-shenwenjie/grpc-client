#ifndef PROVIDER_H
#define PROVIDER_H

#include "../grpc/adaptee.h"
#include "manager.grpc.pb.h"

using namespace std;
using zdautomotive::protobuf::ZDService;
using zdautomotive::protobuf::SERVICE_TYPE;
using zdautomotive::protobuf::ZDServiceRequest;
using zdautomotive::protobuf::ZDServiceRequestResult;

class Provider : public GrpcProviderAdaptee
{
public:
     Provider();
     ~Provider();
     virtual string getSubscriberId() const;
     virtual string getGrpcHost() const;
     virtual SERVICE_TYPE *getSubscriberServiceTypes() const;
     virtual ZDService *getCurrentServiceStatus() const;
     virtual ZDServiceRequestResult* onSubscriberServiceRequest(ZDServiceRequest&) const;
};

#endif