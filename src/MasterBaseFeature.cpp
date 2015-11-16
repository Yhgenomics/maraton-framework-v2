#include "MasterBaseFeature.h"

MasterBaseFeature::MasterBaseFeature( std::string host , int port )
{
    this->host_ = host;
    this->port_ = port;

    this->service_->listen( this->host_.c_str() , this->port_ );
}

MasterBaseFeature::~MasterBaseFeature()
{

}

void MasterBaseFeature::evt_service_failed( NetworkService * service , size_t status )
{

}

void MasterBaseFeature::evt_service_open_session( NetworkService * service , Session * session )
{
    if ( nodes_index_ >= MAX_MASTER_NODE_SIZE )
    {
        session->close();
        return;
    }

    int i = 0;

    while ( nullptr != nodes_[i++] );

    nodes_[i] = this->create_node( session );

    if ( i > nodes_index_ )
    {
        nodes_index_ = i;
    }
}

void MasterBaseFeature::evt_service_close_session( NetworkService * service , Session * session )
{
    int i = 0; 

    for ( size_t i = 0; i <= this->nodes_index_; i++ )
    {
        if ( nodes_[i]->session() == session )
        {
            nodes_[i] = nullptr;
            if ( i == nodes_index_ )
            {
                nodes_index_--;
            }
            break;
        }
    }
}

void MasterBaseFeature::evt_service_stop( NetworkService * service )
{
}
