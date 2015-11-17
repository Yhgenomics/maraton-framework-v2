#include "MasterBaseFeature.h"

MasterBaseFeature::MasterBaseFeature( std::string host )
    : ClusterFeature( host , MASTER_PORT )
{
    this->service_->listen( this->host_.c_str() , this->port_ );
}

MasterBaseFeature::~MasterBaseFeature()
{

}
