/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-16
* Description:
* * * * * * * * * * * * * * * */

#ifndef MASTER_BASE_FEATURE_H_
#define MASTER_BASE_FEATURE_H_

#include <string>

#include "maraton-framework.h"
#include "ClusterNode.h"

class MasterBaseFeature :
    public Feature
{
public:

    MasterBaseFeature( std::string host , int port );
    virtual ~MasterBaseFeature();

    virtual void evt_service_failed         ( NetworkService* service ,
                                              size_t status ) override ;
    
    virtual void evt_service_open_session   ( NetworkService* service ,
                                              Session* session ) override;
    
    virtual void evt_service_close_session  ( NetworkService* service ,
                                              Session* session ) override;
    
    virtual void evt_service_stop           ( NetworkService* service ) override;

protected:
     
    std::string                     host_                     = "";
    int                             port_                     = 0;
    std::array<UPTR<ClusterNode> , 
              MAX_MASTER_NODE_SIZE> nodes_;
    //UPTR<ClusterNode>           nodes_[MAX_MASTER_NODE_SIZE]    = { 0 };
    int                             nodes_index_              = 0;

    virtual UPTR<ClusterNode>       create_node( Session* s ) { return MAKE_UPTR( ClusterNode , s ); };
};

#endif // !MASTER_BASE_FEATURE_H_
