/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-20
* Description:
* * * * * * * * * * * * * * * */

#ifndef CLUSTER_SESSION_H_
#define CLUSTER_SESSION_H_

#include "Macro.h"
#include "Session.h"
#include "Message.h"
#include "CircleBuffer.h"

NS_MARATON_BEGIN

class ClusterSession :
    public Session
{
public:

    ClusterSession       ( );
    size_t  id           ( );
    void    send_message ( uptr<Message> message );

protected:

    virtual void on_message ( uptr<Message> message ) = 0;
    virtual void on_connect ( )                   override;
    void         on_read    ( uptr<Buffer> data ) override;
    void         on_write   ( uptr<Buffer> data ) override;
    virtual void on_close   ( )                   override;

private:

    enum MessageParseState
    {
        Header = 1,
        Length ,
        Body
    };

    CircleBuffer        buffer_; 
    MessageParseState   state_           = MessageParseState::Header;
    size_t              oringal_size_    = 0;
    size_t              compressed_size_ = 0;
    size_t              id_              = -1;

    void try_pop_message( );
};

NS_MARATON_END

#endif // !CLUSTER_SESSION_H_
