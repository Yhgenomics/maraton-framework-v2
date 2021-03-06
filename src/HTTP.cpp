#include "HTTP.h"

NS_MARATON_BEGIN

Url::Url( std::string url )
{
    this->url_ = url;
    this->parse( this->url_ );
}

Url::~Url( )
{

}

std::string Url::domain( )
{
    return this->domain_;
}

std::string Url::path( )
{
    return this->path_;
}

std::string Url::protocol( )
{
    return this->protocol_;
}

int Url::port( )
{
    return this->port_;
}

void Url::parse( std::string url )
{
    // http://blog.csdn.net/is2120/article/details/6251412
    int state = 0;
    int index = 0;

    do
    {
        switch ( state )
        {
            case 0:
                {
                    if ( url[index] == ':' )
                    {
                        index+=2;
                        state = 1;
                        break;
                    }
                    this->protocol_ += url[index];
                }
                break;
            case 1:
                {
                    if ( url[index] == ':' )
                    {
                        state = 2;
                        break;
                    }

                    if ( url[index] == '/' )
                    {
                        state = 3;
                        break;
                    }

                    this->domain_ += url[index];
                }
                break;
            case 2:
                {
                    if ( url[index] == '/' )
                    {
                        this->port_ = atoi( this->tmp_.c_str( ) );
                        this->tmp_ = "";
                        state = 3;
                        break;
                    }

                    this->tmp_+=url[index];
                }
                break;
            case 3:
                {
                    this->path_+=url[index];
                }
                break;
            default:
                break;
        }

        index++;
    }
    while ( index < url.size( ) );

    if ( this->path_.empty( ) )
    {
        this->port_ = atoi( this->tmp_.c_str( ) );
        this->path_ = "/";
    }
}

HTTPRequest::HTTPRequest( std::string url , std::string method )
{
    Url url_parse( url );

    this->domain_ = url_parse.domain( );
    this->url_    = url_parse.path( );
    this->port_   = url_parse.port( );
    this->method_ = method;
}

HTTPRequest::HTTPRequest( )
{
}

HTTPRequest::~HTTPRequest( )
{
}

void HTTPRequest::write_callback( write_callback_t callback )
{
    this->write_callback_ = callback;
}

void HTTPRequest::content( uptr<Buffer> content )
{
    if ( content == nullptr )
    {
        this->content_length( 0 );
    }
    else
    {
        this->content_length( content->size( ) );
        this->content_ = move_ptr( content );
    }
}

uptr<Buffer> HTTPRequest::content( )
{
    if ( this->content_ == nullptr ) return nullptr;

    return make_uptr( Buffer ,
                      this->content_->data( ) ,
                      this->content_->size( ) );
}

void HTTPRequest::content_length( size_t size )
{
    this->content_length_ = size;
    char buf[32]          = { 0 };
    sprintf( buf , "%lld" , size );
    //ltoa( ( long ) size , buf , 10 );
    this->header( "Content-Length" , std::string( buf ) );
}

size_t HTTPRequest::content_length( )
{
    return this->content_length_;
}

void HTTPRequest::header( std::string key , std::string value )
{
    this->header_[key] = value;
}

std::string HTTPRequest::header( std::string key )
{
    return this->header_[key];
}

void HTTPRequest::parse( uptr<Buffer> data )
{
    if ( data == nullptr )
    {
        return;
    }

    char* pdata = data->data( );
    char* ori_data = data->data( );

    do
    {
        switch ( this->parse_state_ )
        {
            case ParseState::kMethod:
                {
                    if ( *pdata == ' ' )
                    {
                        this->parse_state_ = ParseState::kUrl;
                        break;
                    }

                    this->method_ += *pdata;
                }
                break;
            case ParseState::kUrl:
                {
                    if ( *pdata == ' ' )
                    {
                        this->parse_state_ = ParseState::kProtocol;
                        break;
                    }

                    this->url_ += *pdata;
                }
                break;
            case ParseState::kProtocol:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadKey;
                    }
                }
                break;
            case ParseState::kHeadKey:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kContent;
                        if ( !this->header_["Content-Length"].empty( ) )
                        {
                            this->content_length_ =
                                atoll( this->header_["Content-Length"].c_str( ) );
                        }

                        break;
                    }

                    if ( *pdata == ':' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadValue;
                        break;
                    }

                    this->tmp_key_ += *pdata;
                }
                break;
            case ParseState::kHeadValue:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->header( this->tmp_key_ , this->tmp_value_ );

                        this->tmp_key_     = "";
                        this->tmp_value_   = "";
                        this->parse_state_ = ParseState::kHeadKey;
                        break;
                    }

                    this->tmp_value_ += *pdata;
                }
                break;
            case ParseState::kContent:
                {
                    if ( this->content_length_ > MAX_CIRCLE_BUFFER_SIZE )
                    {
                        return;
                    }
                    else if ( this->content_ == nullptr )
                    {
                        this->content_ = make_uptr( MRT::Buffer , this->content_length_ );
                    }

                    this->content_->push( pdata ,
                                          data->size( ) - ( pdata - ori_data ) );
                    return;
                }
                break;
            default:
                break;
        }

        ++pdata;
    }
    while ( ( pdata - ori_data ) < data->size( ) );
}

uptr<Buffer> HTTPRequest::build_header( )
{
    std::string head = "";
    std::string new_line = "\r\n";

    head += this->method_ + " /" + this->url_ + " HTTP/1.1" + new_line;

    for ( auto kv : this->header_ )
    {
        head += kv.first + ": " + kv.second + new_line;
    }

    head += new_line;

    return make_uptr( Buffer , head );
}

uptr<Buffer> HTTPRequest::build_body( )
{
    if ( this->write_callback_ != nullptr )
    {
        auto result = this->write_callback_( this );
        return move_ptr( result );
    }

    if ( this->content_ != nullptr )
    {
        return make_uptr( Buffer ,
                          this->content_->data( ) ,
                          this->content_->size( ) );
    }

    return nullptr;
}

void* HTTPRequest::data( )
{
    return this->data_;
}

void HTTPRequest::data( void * value )
{
    this->data_ = value;
}

bool HTTPRequest::finish( )
{
    if ( this->content_ == nullptr )
    {
        return false;
    }

    return this->content_->size( ) == this->content_length_;
}

//HTTPResponse::HTTPResponse( size_t status )
//{
//    this->status( status );
//}

HTTPResponse::HTTPResponse( )
{
}

HTTPResponse::~HTTPResponse( )
{
}

void HTTPResponse::read_callback( rep_read_callback_t callback )
{
    this->read_callback_ = callback;
}

void * HTTPResponse::data( )
{
    return this->data_;
}

void HTTPResponse::data( void * value )
{
    this->data_ = value;
}

void HTTPResponse::header( std::string key , std::string value )
{
    this->header_[key] = value;
}

std::string HTTPResponse::header( std::string key )
{
    return this->header_[key];
}

void HTTPResponse::content( uptr<Buffer> content )
{
    if ( content == nullptr ) return;

    this->content_length( content->size( ) );
    this->content_ = move_ptr( content );
}

uptr<Buffer> HTTPResponse::content( )
{
    if ( this->content_ == nullptr ) return nullptr;

    uptr<Buffer> result = make_uptr( Buffer ,
                                     this->content_->data( ) ,
                                     this->content_->size( ) );
    return result;
}

void HTTPResponse::content_length( size_t size )
{
    this->content_length_ = size;
    char buf[16]          = { 0 };
    sprintf(buf ,  "%lld" , size );
    //ltoa( size , buf , 10 );
    this->header( "Content-Length" , std::string( buf ) );
}

size_t HTTPResponse::content_length( )
{
    return this->content_length_;
}

void HTTPResponse::status( size_t code )
{
    this->status_ = code;

    switch ( this->status_ )
    {
        case 200:
            {
                this->status_str_ = "OK";
            }
            break;
        case 404:
            {
                this->status_str_ = "Not Found";
            }
            break;
        default:
            {
                this->status_str_ = "Unknown";
            }
            break;
    }
}

size_t HTTPResponse::status( )
{
    return this->status_;
}

uptr<Buffer> HTTPResponse::build_header( )
{
    std::string head        = "";
    std::string new_line    = "\r\n";
    char tmp[16]            = { 0 };

    sprintf(tmp ,  "%lld" , this->status_ );
    //ltoa( this->status_ , tmp , 10 );
    std::string status_code_str ( tmp );

    head += "HTTP/1.1 " + status_code_str + " " + this->status_str_ + new_line;

    for ( auto kv : this->header_ )
    {
        head += kv.first + ": " + kv.second + new_line;
    }

    head += new_line;

    return make_uptr( Buffer , head );
}

uptr<Buffer> HTTPResponse::build_body( )
{
    if ( this->content_ != nullptr )
    {
        return make_uptr( Buffer ,
                          this->content_->data( ) ,
                          this->content_->size( ) );
    }

    return nullptr;
}

void HTTPResponse::parse( uptr<Buffer> data )
{
    if ( data == nullptr )
    {
        return;
    }

    char* pdata = data->data( );
    char* ori_data = data->data( );

    do
    {
        switch ( this->parse_state_ )
        {
            case ParseState::kProtocol:
                {
                    if ( *pdata == ' ' )
                    {
                        this->parse_state_ = ParseState::kStatusCode;
                        break;
                    }

                    this->protocol_ += *pdata;
                }
                break;
            case ParseState::kStatusCode:
                {
                    if ( *pdata == ' ' )
                    {
                        char buf[16]        = { 0 };
                        this->status_       = atoll( this->status_str_.c_str( ) );
                        this->status_str_   = "";
                        this->parse_state_  = ParseState::kStatus;
                        break;
                    }

                    this->status_str_ += *pdata;
                }
                break;
            case ParseState::kStatus:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadKey;
                    }
                    this->status_str_ += *pdata;
                }
                break;
            case ParseState::kHeadKey:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kContent;
                        break;
                    }

                    if ( *pdata == ':' )
                    {
                        ++pdata;
                        this->parse_state_ = ParseState::kHeadValue;
                        break;
                    }

                    this->tmp_key_ += *pdata;
                }
                break;
            case ParseState::kHeadValue:
                {
                    if ( *pdata == '\r' )
                    {
                        ++pdata;
                        this->header( this->tmp_key_ , this->tmp_value_ );

                        if ( this->tmp_key_ == "Content-Length" )
                        {
                            this->content_length_ = atoll( this->tmp_value_.c_str( ) );
                            this->content_ = make_uptr( Buffer , this->content_length_ );
                            this->content_->zero( );
                        }

                        this->tmp_key_     = "";
                        this->tmp_value_   = "";
                        this->parse_state_ = ParseState::kHeadKey;
                        break;
                    }

                    this->tmp_value_ += *pdata;
                }
                break;
            case ParseState::kContent:
                {
                    if ( this->read_callback_ != nullptr )
                    {
                        size_t delta_size = data->size( ) - ( pdata - ori_data );
                        this->read_callback_( this ,
                                              make_uptr( Buffer ,
                                              pdata ,
                                              delta_size )
                                              );
                    }
                    else
                    {
                        if ( this->content_length_ > MAX_CIRCLE_BUFFER_SIZE )
                        {
                            return;
                        }

                        auto size = scast<int>( pdata - ori_data );

                        this->content_->push( pdata ,
                                              data->size( ) - size );
                    }
                    return;
                }
                break;
            default:
                break;
        }

        ++pdata;
    }
    while ( ( pdata - ori_data ) < data->size( ) );
}
bool HTTPResponse::finish( )
{
    if ( this->content_ == nullptr )
    {
        return false;
    }

    return this->content_->size( ) == this->content_length_;
}
//
//WebRequestSession::WebRequestSession( uptr<HTTPRequest> req )
//{
//    this->req_ = move_ptr( req );
//    this->rep_ = make_uptr( HTTPResponse );
//}
//
//WebRequestSession::~WebRequestSession( ) 
//{
//
//}
//
//uptr<HTTPResponse> WebRequestSession::response( )
//{
//    return move_ptr( this->rep_ );
//}
//
//void WebRequestSession::on_connect( )
//{
//    auto header = this->req_->build_header( );
//    auto body   = this->req_->build_body( );
//
//    this->send( move_ptr( header ) );
//    this->send( move_ptr( body ) );
//}
//
//void WebRequestSession::on_read( uptr<Buffer> data )
//{
//    this->rep_->parse( move_ptr( data ) );
//}
//
//void WebRequestSession::on_write( uptr<Buffer> data )
//{
//
//}
//
//void WebRequestSession::on_close( )
//{
//
//}
//
//WebRequestConnector::WebRequestConnector( uptr<HTTPRequest> req_ ,
//                                          callback_response_t callback)
//    : Connector( req_->domain() , 80 )
//{
//    this->req_                  = move_ptr( req_ );
//    this->callback_response_    = callback;
//}
//
//WebRequestConnector::~WebRequestConnector( )
//{
//
//}
//
//Session * WebRequestConnector::create_session( )
//{
//    return new WebRequestSession( move_ptr( req_ ) );
//}
//
//void WebRequestConnector::on_session_open( Session * session )
//{
//
//}
//
//void WebRequestConnector::on_session_close( Session * session )
//{
//    WebRequestSession* websession = scast<WebRequestSession*>( session );
//    this->callback_response_( move_ptr( websession->response( ) ) );
//}
//
//void WebClient::get( std::string url , 
//                     callback_response_t callback )
//{
//    uptr<HTTPRequest> req          = make_uptr( HTTPRequest , 
//                                                url , 
//                                                "GET" );
//    req->header( "Connection" , "close" );
//
//    uptr<WebRequestConnector> conn = make_uptr( WebRequestConnector ,
//                                                move_ptr( req ) ,
//                                                callback );
//
//    Maraton::instance( )->regist( move_ptr( conn ) );
//} 

NS_MARATON_END

