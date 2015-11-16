#include "ExecutorBaseFeature.h"
  
ExecutorBaseFeature::ExecutorBaseFeature( std::string host , int port )
{
    this->host_ = host;
    this->port_ = port;

    this->service_->connect( this->host_.c_str() , this->port_ );
}

ExecutorBaseFeature::~ExecutorBaseFeature()
{

}

void ExecutorBaseFeature::evt_service_failed( NetworkService * service , size_t status )
{

}

void ExecutorBaseFeature::evt_service_open_session( NetworkService * service , Session * session )
{
    this->node_ = this->create_node( session );
}

void ExecutorBaseFeature::evt_service_close_session( NetworkService * service , Session * session )
{
}

void ExecutorBaseFeature::evt_service_stop( NetworkService * service )
{
}
