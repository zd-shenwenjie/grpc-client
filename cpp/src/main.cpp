#include <iostream>
#include "grpc/worker.h"

using namespace std;


int main()
{
    cout << "Hello, world!" << endl;

    GrpcWorker worker;
    worker.doWork();

    return 0;
}