#include <iostream>
#include "observer.h"

Observer::Observer()
{
    cout << "Observer" << endl;
    p_service_type = new SERVICE_TYPE[SUB_SERVICE_NUM];
    p_service_type[0] = SERVICE_TYPE::SERVICE_SET_NETWORK;
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

ZDServiceSubscriber_SUBSCRIBER_TYPE Observer::getSubscriberType() const
{
    return ZDServiceSubscriber_SUBSCRIBER_TYPE::ZDServiceSubscriber_SUBSCRIBER_TYPE_OBSERVER;
}

SERVICE_TYPE *Observer::getSubscriberServiceTypes() const
{
    return p_service_type;
}

int Observer::getSubscriberServiceTypesNum() const
{
    return SUB_SERVICE_NUM;
}

void Observer::onSubscriberServiceRequest(ZDServiceRequest &req) const
{
    SERVICE_TYPE sub_type = req.type();
    cout << "receive service req :" << sub_type << endl;
    switch (sub_type)
    {
    case SERVICE_TYPE::SERVICE_SET_NETWORK:
        onReceiveSetNetworkReq(req);
        break;
    }
}

void Observer::onReceiveSetNetworkReq(ZDServiceRequest &req) const
{
    cout << "unpack req any data ..." << endl;
    Any *p = req.mutable_data();
    Any reqData = *p;
    BlockFilter filter;
    if (reqData.UnpackTo(&filter))
    {
        cout << "phyid:" << filter.phyid() << endl;
        cout << "vlanid:" << filter.vlanid() << endl;
        cout << "srcip:" << filter.srcip() << endl;
        cout << "dstIp:" << filter.dstip() << endl;
        cout << "srcport:" << filter.srcport() << endl;
        cout << "dstport:" << filter.dstport() << endl;
    }
}