#include <iostream>
#include "observer.h"

Observer::Observer()
{
    cout << "Observer" << endl;
}

Observer::~Observer()
{
    cout << "~Observer" << endl;
}

string Observer::getSubscriberId() const
{
    return "observer:8888";
}

string Observer::getGrpcHost() const
{
    return "localhost:5000";
}

SERVICE_TYPE *Observer::getSubscriberServiceTypes() const
{
    static SERVICE_TYPE arr[1] = {SERVICE_TYPE::SERVICE_SET_NETWORK};
    return arr;
}

void Observer::onSubscriberServiceRequest(ZDServiceRequest& req) const
{
    cout << "Observer onSubscriberServiceRequest" << endl;
}