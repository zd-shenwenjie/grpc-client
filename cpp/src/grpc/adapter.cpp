#include <iostream>
#include "adapter.h"

void GrpcSubscriberAdapter::onSubscriberServiceRequest(string req)
{
    cout << req << endl;
}

string GrpcProviderAdapter::onSubscriberServiceRequest(string req)
{
    cout << req << endl;
    return req;
}