#ifndef GRPC_PROVIDER_H
#define GRPC_PROVIDER_H

#include "../grpc/adaptee.h"

using namespace std;

class Provider : public GrpcProviderAdaptee
{
public:
     Provider();
     ~Provider();
     virtual string getSubscriberId() const;
     virtual string getGrpcHost() const;
     virtual int *getSubscriberServiceTypes() const;
     virtual string *getCurrentServiceStatus() const;
     virtual string onSubscriberServiceRequest(string) const;
};

#endif