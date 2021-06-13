#ifndef PROVIDER_H
#define PROVIDER_H

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