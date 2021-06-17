#ifndef OBSERVER_H
#define OBSERVER_H

#include "../grpc/adaptee.h"
#include "manager.grpc.pb.h"
#include "networkif.grpc.pb.h"

using namespace std;
using google::protobuf::Any;
using zdautomotive::protobuf::BlockFilter;
using zdautomotive::protobuf::ProtocolTag;
using zdautomotive::protobuf::SERVICE_TYPE;
using zdautomotive::protobuf::ZDServiceRequest;
using zdautomotive::protobuf::ZDServiceSubscriber_SUBSCRIBER_TYPE;
using zdautomotive::protobuf::ZDSubscriberStatus;

class Observer : public GrpcSubscriberAdaptee
{
public:
     Observer();
     ~Observer();
     virtual string getSubscriberId() const;
     virtual string getGrpcHost() const;
     virtual ZDServiceSubscriber_SUBSCRIBER_TYPE getSubscriberType() const;
     virtual SERVICE_TYPE *getSubscriberServiceTypes() const;
     virtual int getSubscriberServiceTypesNum() const;
     virtual void onSubscriberServiceRequest(ZDServiceRequest &) const;

private:
     const int SUB_SERVICE_NUM = 1;
     SERVICE_TYPE *p_service_type;
     void onReceiveSetNetworkReq(ZDServiceRequest &) const;
};

#endif