#include <iostream>
#include "provider.h"

Provider ::Provider()
{
    // cout << "Provider" << endl;
    p_service_type = new SERVICE_TYPE[SUB_SERVICE_NUM];
    p_service_type[0] = SERVICE_TYPE::SERVICE_SET_NETWORK;
    p_service_status = new ZDService[SUB_SERVICE_NUM];
    ZDService service;
    service.set_type(SERVICE_TYPE::SERVICE_SET_NETWORK);
    service.set_status(ZDService_SERVICE_STATUS::ZDService_SERVICE_STATUS_ENABLED);
    p_service_status[0] = service;
}

Provider ::~Provider()
{
    // cout << "~Provider" << endl;
    delete[] p_service_type;
    delete[] p_service_status;
}

string Provider ::getSubscriberId() const
{
    return "provider:8888";
}

string Provider ::getGrpcHost() const
{
    return "localhost:5000";
}

ZDServiceSubscriber_SUBSCRIBER_TYPE Provider::getSubscriberType() const
{
    return ZDServiceSubscriber_SUBSCRIBER_TYPE::ZDServiceSubscriber_SUBSCRIBER_TYPE_PROVIDER;
}

SERVICE_TYPE *Provider ::getSubscriberServiceTypes() const
{
    return p_service_type;
}

ZDService *Provider ::getCurrentServiceStatus() const
{
    return p_service_status;
}

int Provider::getSubscriberServiceTypesNum() const
{
    return SUB_SERVICE_NUM;
}

ZDServiceRequestResult Provider::onSubscriberServiceRequest(ZDServiceRequest &req) const
{
    ZDServiceRequestResult result;
    SERVICE_TYPE sub_type = req.type();
    cout << "receive service req :" << sub_type << endl;
    switch (sub_type)
    {
    case SERVICE_TYPE::SERVICE_SET_NETWORK:
        onHandleSetNetworkReq(req, &result);
        break;
    }
    return result;
}

void Provider::onHandleSetNetworkReq(ZDServiceRequest &req, ZDServiceRequestResult *result) const
{
    cout << "unpack req any data ..." << endl;
    Any *p = req.mutable_data();
    Any reqAny = *p;
    BlockFilter filter;
    if (reqAny.UnpackTo(&filter))
    {
        cout << "phyid:" << filter.phyid() << endl;
        cout << "vlanid:" << filter.vlanid() << endl;
        cout << "srcip:" << filter.srcip() << endl;
        cout << "dstIp:" << filter.dstip() << endl;
        cout << "srcport:" << filter.srcport() << endl;
        cout << "dstport:" << filter.dstport() << endl;
    }
    ZDResponse res;
    res.set_code(ZDResponse_ERROR_CODE::ZDResponse_ERROR_CODE_OK);
    res.set_message("send req successfully.");
    ProtocolTag tag;
    tag.set_vlanid(456);
    tag.set_srcport(456);
    tag.set_dstport(466);
    tag.set_tag("465");
    Any resAny;
    resAny.PackFrom(tag);
    res.mutable_data()->MergeFrom(resAny);
    result->mutable_request()->MergeFrom(req);
    result->mutable_response()->MergeFrom(res);
}