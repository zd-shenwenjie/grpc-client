#ifndef GRPC_OBSERVER_H
#define GRPC_OBSERVER_H

#include "../grpc/adaptee.h"

using namespace std;

class Observer : public GrpcSubscriberAdaptee
{
public:
     Observer();
     ~Observer();
     virtual string getSubscriberId() const;
     virtual string getGrpcHost() const;
     virtual int *getSubscriberServiceTypes() const;
     virtual void onSubscriberServiceRequest(string req) const;
};

#endif