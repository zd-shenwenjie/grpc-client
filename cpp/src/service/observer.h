#ifndef OBSERVER_H
#define OBSERVER_H

#include "../grpc/adaptee.h"
#include "manager.grpc.pb.h"

using namespace std;
using zdautomotive::protobuf::SERVICE_TYPE;
using zdautomotive::protobuf::ZDSubscriberStatus;
using zdautomotive::protobuf::ZDServiceRequest;

class Observer : public GrpcSubscriberAdaptee
{
public:
     Observer();
     ~Observer();
     virtual string getSubscriberId() const;
     virtual string getGrpcHost() const;
     virtual SERVICE_TYPE *getSubscriberServiceTypes() const;
     virtual void onSubscriberServiceRequest(ZDServiceRequest& ) const;
};

#endif