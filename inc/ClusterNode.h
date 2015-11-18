/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-16
* Description:
* * * * * * * * * * * * * * * */

#ifndef CLUSTER_SESSION_H_
#define CLUSTER_SESSION_H_

#include "maraton.h"
#include "Session.h"
#include "Message.h"
#include "IClusterNode.h"

class ClusterNode;

class IClusterNodeSubscribe
{
public:

    virtual void evt_clustersession_close( ClusterNode* cluster ) { };
};

class ClusterNode :
    public IClusterNode ,
    public ISessionSubscribe ,
    public EventNotifier<IClusterNodeSubscribe>
{
public :

    ClusterNode                             ( Session* session );
    virtual ~ClusterNode                    ( );
    virtual void close                      ( );
    virtual void send                       ( UPTR<Message> message );

    void         evt_session_close          ( Session* )                  override;
    void         evt_session_receive_data   ( Session*, UPTR<Buffer> buffer ) override;
    void         evt_session_sent_complete  ( Session*, size_t size )     override;

    Session*     session                    ( ){ return this->session_; };
    size_t       id                         ( );

protected:

    virtual void on_message                 ( UPTR<Message> msg );
    virtual void on_close                   ( );
    virtual void parse_message              ( );

private:

    enum ParseState
    {
        kFlag ,
        kHead ,
        kBody
    };

    Session*        session_            = nullptr;
    CircleBuffer    circle_buffer_;

    ParseState      parse_state_        = ParseState::kFlag;
    size_t          compressed_length_  = 0;
    size_t          oringal_length_     = 0;

};

#endif // !CLUSTER_SESSION_H_
