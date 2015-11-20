/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-19
* Description:
* * * * * * * * * * * * * * * */

#ifndef MRT_SESSION_H_
#define MRT_SESSION_H_

#include "Macro.h"
#include "Buffer.h"
#include "uv.h"

NS_MARATON_BEGIN

class Operator;

class Session
{
public:

    Session( );
    virtual ~Session();

    void         close  ( );

protected:
    
    Operator* parent_ =  nullptr;

    virtual void send       ( uptr<Buffer> data );
    virtual void on_connect ( )                   { };
    virtual void on_read    ( uptr<Buffer> data ) { };
    virtual void on_write   ( uptr<Buffer> data ) { };
    virtual void on_close   ( )                   { };

private:
    
    enum SessionMode
    {
        Unknown = 0 ,
        Server ,
        Client 
    };

    struct write_token_t
    {
        uv_write_t *        writer;
        uv_buf_t *          buffer;
        Session *           session;
    };

    uv_tcp_t*   uv_tcp_  = nullptr;
    SessionMode session_mode_ = SessionMode::Unknown;
    
    void uv_on_accepted           ( Operator* opt );
    void uv_on_connected          ( Operator* opt );
    void uv_on_close              ( );

    static void uv_alloc_callback ( uv_handle_t * handle ,
                                    size_t suggested_size ,
                                    uv_buf_t * buf  );
                         
    static void uv_read_callback  ( uv_stream_t * stream ,
                                    ssize_t nread ,
                                    const uv_buf_t * buf);
    static void uv_close_callback ( uv_handle_t* handler );

    static void uv_write_callback ( uv_write_t * req, int status );

    friend class Operator;
    friend class Listener;
    friend class Connector;
};

NS_MARATON_END

#endif // !MRT_SESSION_H_
