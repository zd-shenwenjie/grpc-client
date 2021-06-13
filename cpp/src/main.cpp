#include <iostream>
#include "./grpc/adapter.h"
#include "./grpc/adaptee.h"
#include "./service/observer.h"
#include "./service/provider.h"

using namespace std;

int main()
{
    cout << "Hello, world!" << endl;

    GrpcAdaptee *observer = new Observer();
    // GrpcAdaptee *provider = new Provider();

    GrpcWorker *worker = new GrpcAdapter(observer);
    // GrpcWorker *worker = new GrpcAdapter(provider);
    worker->doWork();
    worker->stopWork();

    delete worker;
    delete observer;
    // delete provider;
    return 0;
}