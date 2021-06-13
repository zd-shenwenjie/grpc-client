#include <iostream>
#include "adaptee.h"

GrpcAdaptee::GrpcAdaptee()
{
    cout << "GrpcAdaptee" << endl;
}

GrpcAdaptee::~GrpcAdaptee()
{
    cout << "~GrpcAdaptee" << endl;
}

GrpcSubscriberAdaptee::GrpcSubscriberAdaptee()
{
    cout << "GrpcSubscriberAdaptee" << endl;
}

GrpcSubscriberAdaptee::~GrpcSubscriberAdaptee()
{
    cout << "~GrpcSubscriberAdaptee" << endl;
}

GrpcProviderAdaptee::GrpcProviderAdaptee()
{
    cout << "GrpcProviderAdaptee" << endl;
}

GrpcProviderAdaptee::~GrpcProviderAdaptee()
{
    cout << "~GrpcProviderAdaptee" << endl;
}

int GrpcSubscriberAdaptee::getSubscriberType() const
{
    return 0;
}

int GrpcProviderAdaptee::getSubscriberType() const
{
    return 1;
}
