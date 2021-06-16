#include <iostream>
#include "provider.h"

Provider ::Provider()
{
    cout << "Provider" << endl;
}

Provider ::~Provider()
{
    cout << "~Provider" << endl;
}

string Provider ::getSubscriberId() const
{
    return "provider:8888";
}

string Provider ::getGrpcHost() const
{
    return "localhost:5000";
}

SERVICE_TYPE* Provider ::getSubscriberServiceTypes() const
{
    static SERVICE_TYPE arr[1] = {SERVICE_TYPE::SERVICE_SET_NETWORK};
    return arr;
}

ZDService* Provider ::getCurrentServiceStatus()const
{
    return NULL;
}

ZDServiceRequestResult* Provider::onSubscriberServiceRequest(ZDServiceRequest& req)const
{
    cout << "Provider onSubscriberServiceRequest" << endl;
    return NULL;
}