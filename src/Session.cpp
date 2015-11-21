#include "Session.h"
#include "Operator.h"

NS_MARATON_BEGIN

Session::Session( )
{
}

Session::~Session( )
{

}

void Session::close( )
{
    this->opt_->close_session( this );
}

void Session::send( uptr<Buffer> data )
{
    if ( data == nullptr )
    {
        return;
    }

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
    LOG_DEBUG_UV( r );
    LOG_DEBUG( "Send %lld bytes" , data->size( ) );
}

void Session::uv_write_callback( uv_write_t * req , int status )
{
    write_token_t* write_token = scast<write_token_t*>( req->data );
    
    if( write_token == nullptr )
    {
        LOG_DEBUG( "write_token is nullptr!" );
        return;
    }

    uv_buf_t* buffer           = write_token->buffer;

    if ( status < 0 )
    {
        LOG_DEBUG_UV( status );
        SAFE_DELETE( buffer->base );
        SAFE_DELETE( buffer );
        SAFE_DELETE( write_token );
        SAFE_DELETE( req );
        return;
    }

    write_token->session->on_write( make_uptr( Buffer , 
                                    write_token->buffer->base , 
                                    write_token->buffer->len ) );

    SAFE_DELETE( buffer->base );
    SAFE_DELETE( buffer );
    SAFE_DELETE( write_token );
    SAFE_DELETE( req );
} 

NS_MARATON_END
