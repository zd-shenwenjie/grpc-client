#ifndef GRPC_ADAPTEE_H
#define GRPC_ADAPTEE_H

#include <string>
using namespace std;

class GrpcSubscriberAdaptee
{
public:
    GrpcSubscriberAdaptee();
    virtual ~GrpcSubscriberAdaptee();
    int getSubscriberType();
    virtual string getSubscriberId() const = 0;
    virtual string getGrpcHost() const = 0;
    virtual int *getSubscriberServiceTypes() const = 0;
    virtual void onSubscriberServiceRequest(string) const = 0;
};

class GrpcProviderAdaptee
{
public:
    GrpcProviderAdaptee();
    virtual ~GrpcProviderAdaptee();
    int getSubscriberType();
    virtual string getSubscriberId() const = 0;
    virtual string getGrpcHost() const = 0;
    virtual int *getSubscriberServiceTypes() const = 0;
    virtual string *getCurrentServiceStatus() const = 0;
    virtual string onSubscriberServiceRequest(string) const = 0;
};

#endif