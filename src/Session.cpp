#include "Session.h"
#include "Operator.h"

NS_MARATON_BEGIN

void Session::send( uptr<Buffer> data )
{
    write_token_t* write_token  = new write_token_t;
    write_token->writer         = new uv_write_t();
    write_token->buffer         = new uv_buf_t();
    write_token->buffer->base   = new char[data->size()] { 0 };
    write_token->buffer->len    = data->size();
    write_token->writer->data   = write_token;
    write_token->session        = this;

    memcpy( write_token->buffer->base, data->data(), data->size() );
    
    auto r  = uv_write( write_token->writer, 
                        (uv_stream_t*) &this->uv_tcp_, 
                        write_token->buffer, 
                        1,  
                        Session::uv_write_callback );
    if ( r != 0 )
    {
        LOG_DEBUG("uv errors:%s",UV_ERROR(r));
    }
}

void Session::uv_write_callback( uv_write_t * req , int status )
{
    if ( status < 0 )
    {
        LOG_DEBUG_UV( status );
        return;
    }
    write_token_t* write_token = scast<write_token_t*>( req->data );

    uv_buf_t* buffer            = write_token->buffer;

    write_token->session->on_write( make_uptr( Buffer , write_token->buffer->base , write_token->buffer->len ) );

    SAFE_DELETE( buffer->base );
    SAFE_DELETE( buffer );
    SAFE_DELETE( write_token );
    SAFE_DELETE( req );
}

void Session::close()
{
  
}

void Session::uv_on_connected( Operator * opt )
{   
    this->parent_ = opt;
    this->on_connect();

    int result = uv_read_start( (uv_stream_t*) &this->uv_tcp_ , Session::uv_alloc_callback , Session::uv_read_callback);
    LOG_DEBUG_UV( result );
}             
              
void Session::uv_on_close()
{   
    this->on_close();
    this->parent_->on_close_session( this );
}             
               
void Session::uv_alloc_callback( uv_handle_t * handle , size_t suggested_size , uv_buf_t * buf )
{
    buf->base = new char[suggested_size];
    buf->len  = suggested_size;
}

void Session::uv_read_callback( uv_stream_t * stream , ssize_t nread , const uv_buf_t * buf )
{
    Session* session = scast<Session*>( stream->data );

    if ( session == nullptr )
    {
        LOG_DEBUG( "uv_read_callback: session is nullptr!!" );
        return;
    }

    if ( nread < 0 )
    {
        delete buf->base;
        uv_close( (uv_handle_t*)&session->uv_tcp_ , Session::uv_close_callback);
        return;
    }

    auto data = make_uptr( Buffer , buf->base , nread );

    session->on_read( move_ptr( data ) );

    delete buf->base;
}

void Session::uv_close_callback( uv_handle_t * handler )
{
    Session* session = scast<Session*>( handler->data );

    if ( session == nullptr )
    {
        LOG_DEBUG( "uv_read_callback: session is nullptr!" );
        return;
    }

    session->uv_on_close();

    SAFE_DELETE( session );
}


NS_MARATON_END
