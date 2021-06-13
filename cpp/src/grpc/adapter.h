#ifndef GRPC_ADAPTER_H
#define GRPC_ADAPTER_H
#include "adaptee.h"

using namespace std;

class GrpcWorker
{
public:
    GrpcWorker();
    virtual ~GrpcWorker();
    virtual void doWork() const = 0;
    virtual void stopWork() const = 0;
};
class GrpcAdapter : public GrpcWorker
{
public:
    GrpcAdapter(GrpcAdaptee *);
    ~GrpcAdapter();
    virtual void doWork() const;
    virtual void stopWork() const;

private:
    GrpcAdaptee *adaptee;
};

#endif