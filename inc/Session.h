#ifndef SESSION_H_
#define SESSION_H_

#include <functional>

#include "uv.h"
#include "Define.h"
#include "Buffer.h"
#include "EventNotifier.h"

class NetworkService;
class Session;

class ISessionSubscribe
{
public:

    virtual void evt_session_close          ( Session* ) { };
    virtual void evt_session_receive_data   ( Session*, Buffer & buffer ) { };
    virtual void evt_session_sent_complete  ( Session*, size_t size ) { };

};

class Session : 
    public EventNotifier<ISessionSubscribe>
{
public:

    Session( NetworkService* service_ );
    virtual ~Session();
    
    void            close();

    size_t          id      ( ) { return this->session_id_; };
    std::string     host    ( );
    std::string     ip      ( );
    NetworkService* service ( ); 
    virtual void    send    ( Buffer & buffer );

protected:

    virtual void    on_connected( );
    virtual void    on_receive_data( Buffer & buffer );
    virtual void    on_close();
    virtual void    on_send_finish( size_t size );

private:

    struct write_token_t
    {
        uv_write_t *        writer;
        uv_buf_t *          buffer;
        Session *           session;
    };

    NetworkService*                service_                = nullptr;
    uv_tcp_t*               uv_tcp_                 = nullptr;
    uv_connect_t*           uv_connect_             = nullptr;
    char*                   recive_buffer_          = nullptr;

    size_t                  session_id_             = 0;

    static void             uv_prcoess_write_callback( uv_write_t* req, int status );
    static size_t           create_session_id( );

    friend class NetworkService;
};

#endif // SESSION_H_
