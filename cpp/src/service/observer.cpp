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

int *Observer::getSubscriberServiceTypes() const
{
    static int arr[3] = {1, 2, 3};
    return arr;
}

void Observer::onSubscriberServiceRequest(string req) const
{
    cout << "Observer onSubscriberServiceRequest" << endl;
}