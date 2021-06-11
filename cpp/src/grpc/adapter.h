#include <string>
using namespace std;

class GrpcSubscriberAdapter
{
public:
    int subscriberType;
    virtual string getSubscriberId() = 0;
    virtual string getGrpcHost() = 0;
    virtual int getSubscriberServiceTypes() = 0;
    void onSubscriberServiceRequest(string);
};

class GrpcProviderAdapter : GrpcSubscriberAdapter
{
public:
    virtual string getCurrentServiceStatus() = 0;
    string onSubscriberServiceRequest(string);
};