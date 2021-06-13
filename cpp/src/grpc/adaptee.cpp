#include <iostream>
#include "adaptee.h"

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

int GrpcSubscriberAdaptee::getSubscriberType()
{
    return 0;
}

int GrpcProviderAdaptee::getSubscriberType()
{
    return 1;
}
