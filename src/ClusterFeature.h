/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-17
* Description:
* * * * * * * * * * * * * * * */

#ifndef CLUSTER_FEATURE_H_
#define CLUSTER_FEATURE_H_

#include "maraton-framework.h"
#include "Feature.h"
#include "ClusterNode.h"

class ClusterFeature :
    public Feature
{
public:

    ClusterFeature( std::string host , int port );
    virtual ~ClusterFeature();

    void evt_service_failed         ( NetworkService* service ,
                                      size_t status ) override ;
    
    void evt_service_open_session   ( NetworkService* service ,
                                      Session* session ) override;
    
    void evt_service_close_session  ( NetworkService* service ,
                                      Session* session ) override;
    
    void evt_service_stop           ( NetworkService* service ) override;

protected:
     
    std::string                     host_                     = "";
    int                             port_                     = 0;
    std::array<UPTR<ClusterNode>,  
           MAX_MASTER_NODE_SIZE>    nodes_;
    int                             nodes_index_              = 0;
                                    
    int                             node_array_index_ = 0;
    std::array<SPTR<ClusterNode>,
                          10240>    nodes_array_;

    virtual SPTR<ClusterNode> create_node( Session*  session ) = 0;
    virtual void              on_new_clusternode(  SPTR<ClusterNode> node ) = 0;
    virtual void              on_close_clusternode(  SPTR<ClusterNode> node ) = 0;
};

#endif // !CLUSTER_FEATURE_H_
