#ifndef GRPC_ADAPTEE_H
#define GRPC_ADAPTEE_H

#include <string>
#include "manager.grpc.pb.h"

using namespace std;
using zdautomotive::protobuf::SERVICE_TYPE;
using zdautomotive::protobuf::ZDService;
using zdautomotive::protobuf::ZDServiceRequest;
using zdautomotive::protobuf::ZDSubscriberStatus;
using zdautomotive::protobuf::ZDServiceSubscriber_SUBSCRIBER_TYPE;
using zdautomotive::protobuf::ZDServiceRequestResult;

class GrpcAdaptee
{
public:
    GrpcAdaptee();
    virtual ~GrpcAdaptee();
    virtual ZDServiceSubscriber_SUBSCRIBER_TYPE getSubscriberType() const = 0;
    virtual string getSubscriberId() const = 0;
    virtual string getGrpcHost() const = 0;
    virtual SERVICE_TYPE *getSubscriberServiceTypes() const = 0;
};

class GrpcSubscriberAdaptee : public GrpcAdaptee
{
public:
    GrpcSubscriberAdaptee();
    virtual ~GrpcSubscriberAdaptee();
    virtual ZDServiceSubscriber_SUBSCRIBER_TYPE getSubscriberType() const;
    virtual void onSubscriberServiceRequest(ZDServiceRequest&) const = 0;
};

class GrpcProviderAdaptee : public GrpcAdaptee
{
public:
    GrpcProviderAdaptee();
    virtual ~GrpcProviderAdaptee();
    virtual ZDServiceSubscriber_SUBSCRIBER_TYPE getSubscriberType() const;
    virtual ZDService *getCurrentServiceStatus() const = 0;
    virtual ZDServiceRequestResult* onSubscriberServiceRequest(ZDServiceRequest&) const = 0;
};

#endif