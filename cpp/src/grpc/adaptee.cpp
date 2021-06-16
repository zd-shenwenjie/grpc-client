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

ZDServiceSubscriber_SUBSCRIBER_TYPE GrpcSubscriberAdaptee::getSubscriberType() const
{
    return  ZDServiceSubscriber_SUBSCRIBER_TYPE::ZDServiceSubscriber_SUBSCRIBER_TYPE_PROVIDER;
}

ZDServiceSubscriber_SUBSCRIBER_TYPE GrpcProviderAdaptee::getSubscriberType() const
{
    return  ZDServiceSubscriber_SUBSCRIBER_TYPE::ZDServiceSubscriber_SUBSCRIBER_TYPE_OBSERVER;
}
