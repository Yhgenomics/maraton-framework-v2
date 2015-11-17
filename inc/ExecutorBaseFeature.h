/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-16
* Description:
* * * * * * * * * * * * * * * */

#ifndef EXECUTOR_FEATURE_H_
#define EXECUTOR_FEATURE_H_

#include <string>
#include "maraton.h"
#include "ClusterNode.h"

class ExecutorBaseFeature :
    public Feature
{
public:

    ExecutorBaseFeature                     ( std::string host, int port );
    ~ExecutorBaseFeature                    ( );


    virtual void evt_service_failed         ( NetworkService* service ,
                                              size_t status ) override ;
    
    virtual void evt_service_open_session   ( NetworkService* service ,
                                              Session* session ) override;
    
    virtual void evt_service_close_session  ( NetworkService* service ,
                                              Session* session ) override;
    
    virtual void evt_service_stop           ( NetworkService* service ) override;

protected:

    std::string                 host_ = "";
    int                         port_ = 0;
    UPTR<ClusterNode>           node_ = nullptr;

    virtual UPTR<ClusterNode>   create_node ( Session* ) = 0;
};

#endif // !EXECUTOR_FEATURE_H_
