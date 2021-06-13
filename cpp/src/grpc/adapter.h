#ifndef GRPC_ADAPTER_H
#define GRPC_ADAPTER_H

#include "../service/observer.h"
#include "../service/provider.h"

using namespace std;

class GrpcWorker
{
public:
    GrpcWorker();
    virtual ~GrpcWorker();
    virtual void doWork() const = 0;
    virtual void stopWork() const = 0;
};

class GrpcObserverAdapter : public GrpcWorker
{
public:
    GrpcObserverAdapter(Observer *);
    ~GrpcObserverAdapter();
    virtual void doWork() const;
    virtual void stopWork() const;

private:
    Observer *observer;
};

class GrpcProviderAdapter : public GrpcWorker
{
public:
    GrpcProviderAdapter(Provider *);
    ~GrpcProviderAdapter();
    virtual void doWork() const;
    virtual void stopWork() const;

private:
    Provider *provider;
};

#endif