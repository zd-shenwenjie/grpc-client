#include "../grpc/adapter.h"
#include <string>

using namespace std;

class GrpcProvider : GrpcProviderAdapter
{
public:
     string getSubscriberId();
     string getGrpcHost() ;
     int getSubscriberServiceTypes();
     string getCurrentServiceStatus();
};