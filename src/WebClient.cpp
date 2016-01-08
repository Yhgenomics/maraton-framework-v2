#include "WebClient.h"

NS_MARATON_BEGIN

WebClient::WebClient( )
{
    this->header( "Connection" , "close" );
    this->header( "Author" , "YHGenomics/Maraton" );
    this->header( "VHTTP" , "Alpha/0.0.1" );
    this->header( "Accept" , "*/*" );
}

void WebClient::get( std::string url ,
                     callback_response_t callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest , 
                                                     url , 
                                                     "GET");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req                          = move_ptr( req );
    token->callback                     = callback;

    this->fill_header( token->req.get( ) );
    
    this->query_dns( move_ptr( token ) );
}

void WebClient::post( std::string url , 
                      std::string data , 
                      callback_response_t callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest ,
                                                     url , 
                                                     "POST");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req                          = move_ptr( req );
    token->callback                     = callback;
    
    this->fill_header   ( token->req.get( ) );
    token->req->content ( make_uptr( Buffer , data ) );
    this->query_dns     ( move_ptr( token ) );
}

void WebClient::post_file( std::string url , 
                           std::string file_token ,
                           FILE * pfile ,
                           callback_response_t callback )
{
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest ,
                                                     url ,
                                                     "POST");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req                          = move_ptr( req );
    token->callback                     = callback;
    
    req->data( pfile );
    this->fill_header( token->req.get( ) );

    fseek( pfile , 0 , SEEK_END );
    size_t file_size = ftell( pfile );
    fseek( pfile , 0 , SEEK_SET );
    
    req->content_length( file_size );

    req->write_callback( [ ] ( HTTPRequest * request )
    { 
        FILE* pfile = scast<FILE*>( request->data( ) );
        char file_buffer[1024 * 1024] = { 0 };
        size_t reads = fread( file_buffer , 
                              1024 * 1024 , 
                              1 , 
                              pfile );
        return make_uptr( Buffer , file_buffer , reads );
    } );

    
    this->query_dns( move_ptr( token ) );
}

void WebClient::dl_file( std::string url ,
                         FILE* pfile ,
                         callback_response_t callback)
{ 
    uptr<HTTPRequest> req               = make_uptr( HTTPRequest , 
                                                     url , 
                                                     "GET");
    uptr<WebClientRequestToken> token   = make_uptr( WebClientRequestToken );
    token->req                          = move_ptr( req );
    token->rep                          = make_uptr( HTTPResponse ); 
    token->callback                     = callback;

    token->rep->read_callback( [pfile] ( HTTPResponse* rep ,
                               uptr<Buffer>  buf )
    {
        fwrite( buf->data( ) , 1 ,  buf->size( ) , pfile );
    });

    this->fill_header( token->req.get( ) );

    this->query_dns( move_ptr( token ) );
}

void WebClient::header( std::string key , std::string value )
{
    this->header_[key] = value;
}

void WebClient::query_dns( uptr<WebClientRequestToken> t )
{
    auto token = t.release( );

    if ( token->rep == nullptr )
        token->rep                  = make_uptr( HTTPResponse );

    token->uv_tcp.data              = token;
    token->uv_connect.data          = token;
    token->addrinfo.ai_family       = PF_INET;
    token->addrinfo.ai_socktype     = SOCK_STREAM;
    token->addrinfo.ai_protocol     = IPPROTO_TCP;
    token->addrinfo.ai_flags        = 0;
    token->uv_getaddrinfo.data      = token;

    int r = uv_getaddrinfo( uv_default_loop( ) ,
                            &token->uv_getaddrinfo ,
                            WebClient::uv_process_resolved ,
                            token->req->domain( ).c_str( ) ,
                            NULL ,
                            &token->addrinfo );
}

void WebClient::fill_header( HTTPRequest * req )
{
    for( auto kv : this->header_ )
    {
        req->header( kv.first , kv.second );
    }
}

void WebClient::uv_send_request( WebClientRequestToken* token )
{
    uptr<Buffer> header_buf     = token->req->build_header( );
    uptr<Buffer> body_buf       = token->req->build_body( );

    write_token_t* write_token  = new write_token_t;
    write_token->writer         = new uv_write_t();
    write_token->buffer         = new uv_buf_t();
    write_token->buffer->base   = new char[header_buf->size() + 1] { 0 };
    write_token->buffer->len    = header_buf->size();
    write_token->writer->data   = write_token;
    write_token->session        = token;

    memcpy( write_token->buffer->base, 
            header_buf->data(), 
            header_buf->size() );
    
    auto r  = uv_write( write_token->writer, 
                        (uv_stream_t*) &token->uv_tcp, 
                        write_token->buffer, 
                        1,  
                        WebClient::uv_write_callback );
    LOG_DEBUG_UV( r ); 
    LOG_DEBUG( "%s" , write_token->buffer->base );

    if ( body_buf != nullptr &&
         body_buf->size() > 0)
    {
        write_token                 = new write_token_t;
        write_token->writer         = new uv_write_t( );
        write_token->buffer         = new uv_buf_t( );
        write_token->buffer->base   = new char[body_buf->size( ) + 1] { 0 };
        write_token->buffer->len    = body_buf->size( );
        write_token->writer->data   = write_token;
        write_token->session        = token;

        memcpy( write_token->buffer->base ,
                body_buf->data( ) ,
                body_buf->size( ) );

        r  = uv_write( write_token->writer ,
                       ( uv_stream_t* ) &token->uv_tcp ,
                       write_token->buffer ,
                       1 ,
                       WebClient::uv_write_callback );
        LOG_DEBUG_UV( r );
    }
}

void WebClient::uv_process_resolved( uv_getaddrinfo_t * req , 
                                     int status , 
                                     addrinfo * res )
{
    int result = 0;
    WebClientRequestToken *
        token = scast<WebClientRequestToken*>( req->data );

    if ( token == nullptr )
    {
        LOG_DEBUG( "operator is nullptr" );
        return;
    }

    if ( status < 0 )
    {
        LOG_DEBUG_UV( status );
        token->invoke_callback( nullptr );
        SAFE_DELETE( token );
        delete res;
        return;
    }

    char ip[17] = { 0 };

    uv_ip4_name( ( struct sockaddr_in* ) res->ai_addr ,
                 ip ,
                 16 );
    delete res;
    uv_ip4_addr( ip ,
                 token->req->port_ ,
                 &token->addr_in );

    token->ip = ip;

    token->req->header( "Host" , token->req->domain_ );

    uv_tcp_init( uv_default_loop( ) , &token->uv_tcp );

    result = uv_tcp_connect( &token->uv_connect ,
                             &token->uv_tcp ,
                             ( const struct sockaddr* ) &token->addr_in ,
                             WebClient::uv_connected_callback );
    if ( result < 0 )
    {
        LOG_DEBUG_UV( result );
        token->invoke_callback( nullptr );
        SAFE_DELETE( token );
    }
}

void WebClient::uv_connected_callback( uv_connect_t * req , int status )
{
    WebClientRequestToken * 
        token = scast<WebClientRequestToken*>( req->data );

    if ( status < 0 )
    {
        LOG_DEBUG_UV( status );
        uv_close( ( uv_handle_t* ) &token->uv_tcp ,
                  WebClient::uv_close_callback );
        return;
    }

    if ( token == nullptr )
    {
        LOG_DEBUG( "WebClientRequestToken is nullptr!" );
        return;
    }

    auto result = uv_read_start( ( uv_stream_t* ) &token->uv_tcp ,
                                 WebClient::uv_alloc_callback ,
                                 WebClient::uv_read_callback );

    WebClient::uv_send_request( token );

    if ( result < 0 )
    {
        LOG_DEBUG_UV( result );
        uv_close( ( uv_handle_t* ) &token->uv_tcp ,
                  WebClient::uv_close_callback );
    }
}

void WebClient::uv_alloc_callback( uv_handle_t * handle , 
                                   size_t suggested_size , 
                                   uv_buf_t * buf )
{
    buf->base = new char[suggested_size];
    buf->len  = suggested_size;
}

void WebClient::uv_read_callback( uv_stream_t * stream ,
                                  ssize_t nread , 
                                  const uv_buf_t * buf )
{
    WebClientRequestToken 
        * token = scast<WebClientRequestToken*>( stream->data );

    if ( token == nullptr )
    {
        LOG_DEBUG( "Session is nullptr!" );
        return;
    }

    if ( nread < 0 )
    {
        LOG_DEBUG_UV( nread );
        delete buf->base ;
        uv_close( ( uv_handle_t* ) &token->uv_tcp ,
                  WebClient::uv_close_callback );
        return;
    }

    token->rep->parse( make_uptr( Buffer , buf->base , nread ) );

    delete buf->base ;
}

void WebClient::uv_write_callback( uv_write_t * req , int status )
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

    SAFE_DELETE( buffer->base );
    SAFE_DELETE( buffer );
    SAFE_DELETE( write_token );
    SAFE_DELETE( req );
}

void WebClient::uv_close_callback( uv_handle_t * handle )
{
    WebClientRequestToken * 
        token = scast<WebClientRequestToken*>( handle->data );

    token->invoke_callback( move_ptr( token->rep ) );

    SAFE_DELETE( token );
}

NS_MARATON_END
