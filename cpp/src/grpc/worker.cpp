#include <iostream>
#include "worker.h"

void GrpcWorker::doWork()
{
    cout << "do work" << endl;
}

void GrpcWorker::stopWork()
{
    cout << "stop work" << endl;
}