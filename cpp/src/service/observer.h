#include "../grpc/adapter.h"
#include <string>

using namespace std;

class GrpcObserver : GrpcSubscriberAdapter
{
public:
     string getSubscriberId();
     string getGrpcHost() ;
     int getSubscriberServiceTypes();
};
