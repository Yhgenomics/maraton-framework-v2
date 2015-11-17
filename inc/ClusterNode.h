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

class ClusterNode;

class IClusterSessionSubscribe
{
public:

    virtual void evt_clustersession_close( ClusterNode* cluster ) { };
};

class ClusterNode :
    public ISessionSubscribe ,
    public EventNotifier<IClusterSessionSubscribe>
{
public :

    ClusterNode                             ( Session* session );
    virtual ~ClusterNode                    ( );
    virtual void close                      ( );
    virtual void send                       ( UPTR<Message> message );

    void         evt_session_close          ( Session* )                  override;
    void         evt_session_receive_data   ( Session*, Buffer & buffer ) override;
    void         evt_session_sent_complete  ( Session*, size_t size )     override;

    Session*     session                    ( ){ return this->session_; };

protected:

    virtual void on_message                 ( UPTR<Message> msg );
    virtual void on_close                   ( );

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

    void            parse_message();
};

#endif // !CLUSTER_SESSION_H_
