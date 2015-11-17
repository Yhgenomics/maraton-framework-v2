#include "ClusterFeature.h"

ClusterFeature::ClusterFeature( std::string host , int port )
{
    this->host_ = host;
    this->port_ = port;

    this->service_ = MAKE_UPTR( NetworkService );
    this->service_->register_notifier( this );
}

ClusterFeature::~ClusterFeature()
{
}

void ClusterFeature::evt_service_failed( NetworkService * service , 
                                         size_t status )
{
}

void ClusterFeature::evt_service_open_session( NetworkService * service , 
                                               Session * session )
{
    auto node = this->create_node( session );

    for ( size_t i = 0; i <= nodes_array_index_; i++ )
    {
        if ( nodes_array_[i] == nullptr )
        {
            nodes_array_[i] = MOVE( node );

            if ( nodes_array_index_ == i )
            {
                nodes_array_index_++;
            }

            this->on_new_clusternode( nodes_array_[i] );

            break;
        }
    }
}

void ClusterFeature::evt_service_close_session( NetworkService * service , 
                                                Session * session )
{
    for ( size_t i = 0; i <= nodes_array_index_; i++ )
    {
        if ( nodes_array_[i]!= nullptr &&
             nodes_array_[i]->session() == session )
        { 
            if ( nodes_array_index_ == i )
            {
                nodes_array_index_--;
            } 
            this->on_close_clusternode( nodes_array_[i] );
            nodes_array_[i] = nullptr;
            break;
        }
    }
}

void ClusterFeature::evt_service_stop( NetworkService * service )
{
}
