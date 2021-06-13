#include <iostream>
#include "grpc/adapter.h"
#include "./service/observer.h"
#include "./service/provider.h"

using namespace std;

int main()
{
    cout << "Hello, world!" << endl;

    Observer *observer = new Observer();
    GrpcWorker * worker1 = new GrpcObserverAdapter(observer);
    worker1->doWork();
    worker1->stopWork();
    delete worker1;
    delete observer;

    Provider *provider = new Provider();
    GrpcWorker * worker2 = new GrpcProviderAdapter(provider);
    worker2->doWork();
    worker2->stopWork();
    delete worker2;
    delete provider;

    return 0;
}