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

int* Provider ::getSubscriberServiceTypes() const
{
    static int arr[3] = {1,2,3};
    return arr;
}

string* Provider ::getCurrentServiceStatus()const
{
    static string arr[3] = {"1","2","3"};
    return arr;
}

string Provider::onSubscriberServiceRequest(string req)const
{
    cout << "Provider onSubscriberServiceRequest" << endl;
    return req;
}