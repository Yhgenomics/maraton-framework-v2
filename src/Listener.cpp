#include "Listener.h"

NS_MARATON_BEGIN 
 
Listener::Listener( std::string addr , int port )
    : Operator( addr , port )
{
    this->uv_tcp_.data = this;
}

Listener::~Listener()
{

}

void Listener::do_work()
{
    auto result = uv_tcp_init( this->uv_loop_ ,
                               &this->uv_tcp_ );
    LOG_DEBUG_UV( result );
      
    result = uv_tcp_bind( &this->uv_tcp_ ,
                          ( const struct sockaddr* )&this->addr_in_ ,
                          0 );
    LOG_DEBUG_UV( result );

    result = uv_listen  ( ( uv_stream_t* ) &this->uv_tcp_ ,
                          MAX_CONNECTION_SIZE ,
                          Listener::uv_new_connection_callback );
    LOG_DEBUG_UV( result );
} 
 
void Listener::uv_new_connection_callback( uv_stream_t * server , int status )
{
    Listener* listener = scast<Listener*>( server->data );

    if ( status < 0 )
    {
        LOG_DEBUG_UV( status );
        return;
    }

    if ( listener == nullptr )
    {
        LOG_DEBUG( "Listener is nullptr!" );
        return;
    }

    auto session = listener->create_session( );

    uv_tcp_init             ( listener->uv_loop_ , 
                              &session->uv_tcp_ );

    session->uv_tcp_.data   = session;
    session->opt_           = listener;

    auto r = uv_accept( server , ( uv_stream_t* ) &session->uv_tcp_ );
    
    if ( r == 0 )
    {
        //for ( size_t i = 0; i <= listener->session_list_index_; i++ )
        //{
        //    if ( listener->session_list_[i] == nullptr )
        //    {
        //        listener->session_list_[i] = session;
        //        if ( listener->session_list_index_ == i )
        //        {
        //            listener->session_list_index_ = 
        //                ( listener->session_list_index_ + 1 ) %
        //                MAX_CONNECTION_SIZE;
        //        }
        //        break;
        //    }
        //}

        r = uv_read_start( (uv_stream_t*)&session->uv_tcp_ , 
                           Listener::uv_alloc_callback , 
                           Listener::uv_read_callback );
        LOG_DEBUG_UV( r );

        listener->on_session_open( session );
        session->on_connect( );
    }
    else
    {
        uv_close( ( uv_handle_t* ) &session->uv_tcp_ , 
                  Listener::uv_close_callback );
    }
}

void Listener::uv_alloc_callback( uv_handle_t * handle , 
                                  size_t suggested_size , 
                                  uv_buf_t * buf )
{
    buf->base = new char[suggested_size];
    buf->len  = suggested_size;
}

void Listener::uv_read_callback( uv_stream_t * stream , 
                                 ssize_t nread , 
                                 const uv_buf_t * buf )
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
        uv_close( ( uv_handle_t* ) &session->uv_tcp_ , 
                  Listener::uv_close_callback );
        return;
    }

    uptr<Buffer> pbuf = make_uptr( Buffer , buf->base , nread );
    session->on_read( move_ptr( pbuf ) );

    delete buf->base;
}

void Listener::uv_close_callback( uv_handle_t * handle )
{
    Session* session = scast<Session*>( handle->data );
   
    if ( session == nullptr )
    {
        LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    Listener* listener = scast<Listener*>( session->opt_ );

    if ( listener == nullptr )
    {
        LOG_DEBUG( "Listener is nullptr!" );
        return;
    }

    session->on_close           ( );
    listener->on_session_close  ( session );

    //for ( size_t i = 0; i < listener->session_list_index_; i++ )
    //{
    //    if( listener->session_list_[i] == session )
    //    {
    //        SAFE_DELETE( listener->session_list_[i] );
    //        if( (listener->session_list_index_ - 1) == i )
    //        {
    //            --listener->session_list_index_;
    //        }
    //        break;
    //    }
    //} 
}

void Listener::close_session( Session * session )
{
    uv_close( ( uv_handle_t* ) &session->uv_tcp_ , 
              Listener::uv_close_callback );
}
 
NS_MARATON_END
