#ifndef GRPC_ADAPTEE_H
#define GRPC_ADAPTEE_H

#include <string>
using namespace std;

class GrpcAdaptee
{
public:
    GrpcAdaptee();
    virtual ~GrpcAdaptee();
    virtual int getSubscriberType() const = 0;
    virtual string getSubscriberId() const = 0;
    virtual string getGrpcHost() const = 0;
    virtual int *getSubscriberServiceTypes() const = 0;
};

class GrpcSubscriberAdaptee : public GrpcAdaptee
{
public:
    GrpcSubscriberAdaptee();
    virtual ~GrpcSubscriberAdaptee();
    virtual int getSubscriberType() const;
    virtual void onSubscriberServiceRequest(string) const = 0;
};

class GrpcProviderAdaptee : public GrpcAdaptee
{
public:
    GrpcProviderAdaptee();
    virtual ~GrpcProviderAdaptee();
    virtual int getSubscriberType() const;
    virtual string *getCurrentServiceStatus() const = 0;
    virtual string onSubscriberServiceRequest(string) const = 0;
};

#endif