#include <iostream>
#include "adapter.h"
#include "../service/observer.h"
#include "../service/provider.h"

GrpcWorker::GrpcWorker()
{
    cout << "GrpcWorker" << endl;
}

GrpcWorker::~GrpcWorker()
{
    cout << "~GrpcWorker" << endl;
}

GrpcAdapter::GrpcAdapter(GrpcAdaptee *p)
{
    cout << "GrpcAdapter" << endl;
    adaptee = p;
}

GrpcAdapter::~GrpcAdapter()
{
    cout << "~GrpcAdapter" << endl;
}

void GrpcAdapter::doWork() const
{
    cout << "observer do work" << endl;
    if (adaptee != NULL)
    {
        cout << adaptee->getSubscriberType() << endl;
        cout << adaptee->getSubscriberId() << endl;
        int subscriberType = adaptee->getSubscriberType();
        if (subscriberType)
        {
            string str = ((Provider *)adaptee)->onSubscriberServiceRequest("some req2");
            cout << str << endl;
        }
        else
        {
            ((Observer *)adaptee)->onSubscriberServiceRequest("some req1");
        }
    }
}

void GrpcAdapter::stopWork() const
{
    cout << "observer stop work" << endl;
}