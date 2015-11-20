#include "Connector.h"
#include "Maraton.h"

NS_MARATON_BEGIN

Connector::Connector( std::string addr , int port )
    : Operator( addr , port )
{

}

Connector::~Connector( )
{
    SAFE_DELETE( this->session_ );
}

void Connector::do_work( )
{
    this->session_          = this->create_session( );
    this->session_->opt_    = this;

    uv_tcp_init( this->uv_loop_ ,
                 &this->session_->uv_tcp_ );

    this->session_->uv_tcp_.data    = this->session_;
    this->uv_connect_.data          = this;

    auto result = uv_tcp_connect( &this->uv_connect_ ,
                                  &this->session_->uv_tcp_ ,
                                  ( const struct sockaddr* ) &this->addr_in_ ,
                                  Connector::uv_connected_callback );
    LOG_DEBUG_UV( result );
}

void Connector::uv_connected_callback( uv_connect_t * req , int status )
{
    Connector* opt = scast<Connector*>( req->data );

    if ( status < 0 )
    {
        LOG_DEBUG_UV( status );
        uv_close( ( uv_handle_t* ) &opt->session_->uv_tcp_ ,
                  Connector::uv_close_callback );
        return;
    }

    if ( opt == nullptr )
    {
        LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    opt->on_session_open        ( opt->session_ );
    opt->session_->on_connect   ( );

    auto result = uv_read_start( ( uv_stream_t* ) &opt->session_->uv_tcp_ ,
                                 Connector::uv_alloc_callback ,
                                 Connector::uv_read_callback );
    LOG_DEBUG_UV( result );
}

void Connector::uv_alloc_callback( uv_handle_t * handle , size_t suggested_size , uv_buf_t * buf )
{
    buf->base = new char[suggested_size];
    buf->len  = suggested_size;
}

void Connector::uv_read_callback( uv_stream_t * stream , ssize_t nread , const uv_buf_t * buf )
{
    Session* session = scast<Session*>( stream->data );

    if ( session == nullptr )
    {
        LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    if ( nread < 0 )
    {
        LOG_DEBUG_UV( nread );
        uv_close( ( uv_handle_t* ) &session->uv_tcp_ , Connector::uv_close_callback );
        return;
    }

    uptr<Buffer> pbuf = make_uptr( Buffer , buf->base , nread );
    session->on_read( move_ptr( pbuf ) );

    delete buf->base;
}
void Connector::uv_close_callback( uv_handle_t * handle )
{
    Session* session = scast<Session*>( handle->data );

    if ( session == nullptr )
    {
        LOG_DEBUG( "Session is nullptr!" );
        return;
    }
    auto opt = scast<Connector*>( session->opt_ );

    session->on_close( );
    opt->on_session_close( session );
    Maraton::instance( )->unregist( opt );
} 

void Connector::close_session( Session * session )
{
    uv_close( ( uv_handle_t* ) &session->uv_tcp_ , Connector::uv_close_callback );
}

NS_MARATON_END
