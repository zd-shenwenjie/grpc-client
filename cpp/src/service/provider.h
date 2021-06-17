#ifndef PROVIDER_H
#define PROVIDER_H

#include "../grpc/adaptee.h"
#include "manager.grpc.pb.h"
#include "networkif.grpc.pb.h"

using namespace std;
using google::protobuf::Any;
using zdautomotive::protobuf::BlockFilter;
using zdautomotive::protobuf::ProtocolTag;
using zdautomotive::protobuf::SERVICE_TYPE;
using zdautomotive::protobuf::ZDResponse;
using zdautomotive::protobuf::ZDResponse_ERROR_CODE;
using zdautomotive::protobuf::ZDService;
using zdautomotive::protobuf::ZDService_SERVICE_STATUS;
using zdautomotive::protobuf::ZDServiceRequest;
using zdautomotive::protobuf::ZDServiceRequestResult;
using zdautomotive::protobuf::ZDServiceSubscriber_SUBSCRIBER_TYPE;

class Provider : public GrpcProviderAdaptee
{
public:
     Provider();
     ~Provider();
     virtual string getSubscriberId() const;
     virtual string getGrpcHost() const;
     virtual ZDServiceSubscriber_SUBSCRIBER_TYPE getSubscriberType() const;
     virtual SERVICE_TYPE *getSubscriberServiceTypes() const;
     virtual int getSubscriberServiceTypesNum() const;
     virtual ZDService *getCurrentServiceStatus() const;
     virtual ZDServiceRequestResult onSubscriberServiceRequest(ZDServiceRequest &) const;

private:
     const int SUB_SERVICE_NUM = 1;
     SERVICE_TYPE *p_service_type;
     ZDService *p_service_status;
     void onHandleSetNetworkReq(ZDServiceRequest &, ZDServiceRequestResult *) const;
};

#endif