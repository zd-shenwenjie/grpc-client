#include <iostream>
#include "adapter.h"

GrpcWorker::GrpcWorker()
{
    cout << "GrpcWorker" << endl;
}

GrpcWorker::~GrpcWorker()
{
    cout << "~GrpcWorker" << endl;
}

GrpcObserverAdapter::GrpcObserverAdapter(Observer *adaptee)
{
    observer = adaptee;
    cout << "GrpcObserverAdapter" << endl;
}

GrpcObserverAdapter::~GrpcObserverAdapter()
{
    cout << "~GrpcObserverAdapter" << endl;
}

void GrpcObserverAdapter::doWork() const
{
    cout << "observer do work" << endl;
    if (observer != NULL)
    {
        cout << observer->getSubscriberType() << endl;
        cout << observer->getSubscriberId() << endl;
    }
}

void GrpcObserverAdapter::stopWork() const
{
    cout << "observer stop work" << endl;
}

GrpcProviderAdapter::GrpcProviderAdapter(Provider *adaptee)
{
    provider = adaptee;
    cout << "GrpcProviderAdapter" << endl;
}

GrpcProviderAdapter::~GrpcProviderAdapter()
{
    cout << "~GrpcProviderAdapter" << endl;
}

void GrpcProviderAdapter::doWork() const
{
    cout << "provider do work" << endl;
    if (provider != NULL)
    {
        cout << provider->getSubscriberType() << endl;
        cout << provider->getSubscriberId() << endl;
    }
}

void GrpcProviderAdapter::stopWork() const
{
    cout << "provider stop work" << endl;
}