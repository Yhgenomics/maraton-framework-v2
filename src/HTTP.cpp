#include "HTTP.h"

NS_MARATON_BEGIN

HTTPRequest::HTTPRequest( std::string path , std::string method )
{
    this->url_    = path;
    this->method_ = method;

    this->header( "Server" , "YHGenomics/Maraton" );
    this->header( "VHTTP" , "Alpha/0.0.1" );
}

void HTTPRequest::write_callback( write_callback_t callback )
{
    this->write_callback_ = callback;
}

void HTTPRequest::content( uptr<Buffer> content )
{
    this->content_ = move_ptr( content );
    this->content_length( content->size( ) );
}

void HTTPRequest::content_length( size_t size )
{
    this->content_length_ = size;
    char buf[32]          = { 0 };
    ltoa( (long)size , buf , 10 );
    this->header( "Content-Length" , std::string( buf ) );
}

void HTTPRequest::header( std::string key , std::string value )
{
    this->header_[key] = value; 
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
                            this->content_length_ = atoll( this->tmp_value_.c_str( ) );;
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
                    if ( this->content_length_ > MAX_CIRCLE_BUFFER_SIZE )
                    {
                        return;
                    }
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

    head += this->method_ + " " + this->url_ + " HTTP/1.1" + new_line;

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
        return make_uptr( Buffer , this->content_->data( ) , this->content_->size( ) );
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

HTTPResponse::HTTPResponse( size_t status )
{
    this->status( status );
}

HTTPResponse::HTTPResponse( )
{
}

void HTTPResponse::read_callback( read_callback_t callback )
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
    this->content_ = move_ptr( content );
}

void HTTPResponse::content_length( size_t size )
{
    this->content_length_ = size;
    char buf[16]          = { 0 };
    ltoa( size , buf , 10 );
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
    return size_t( );
}

uptr<Buffer> HTTPResponse::build_header( )
{
    std::string head        = "";
    std::string new_line    = "\r\n";
    char tmp[16]            = { 0 };

    ltoa( this->status_ , tmp , 10 );
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
    if ( this->read_callback_ != nullptr )
    {
        return this->read_callback_( this );
    }

    if ( this->content_ != nullptr )
    {
        return make_uptr( Buffer , this->content_->data( ) , this->content_->size( ) );
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
                            this->content_length_ = atoll( this->tmp_value_.c_str( ) );;
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
                    if ( this->content_length_ > MAX_CIRCLE_BUFFER_SIZE )
                    {
                        return;
                    }
                    circle_buffer_.push( pdata , data->size( ) - ( pdata - ori_data ) );
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
 
WebRequestSession::WebRequestSession( sptr<HTTPRequest> req )
{
    this->req_ = move_ptr( req );
    this->rep_ = make_uptr( HTTPResponse );
}

void WebRequestSession::on_connect( )
{
    this->req_->header( "Host" , this->parent_->ip() ); 
    
    auto header = this->req_->build_header( );
    auto body   = this->req_->build_body( );

    this->send( move_ptr( header ) );
    this->send( move_ptr( body ) );
}

void WebRequestSession::on_read( uptr<Buffer> data )
{
    this->recived_length+=data->size( );

    this->rep_->parse( move_ptr( data ) );

    if ( rep_->content_length( ) <= recived_length )
    {
        this->close( );
    }
}


WebConnector::WebConnector( std::string addr, 
                            sptr<HTTPRequest> req ,
                            callback_response_t cb )
    : Connector( addr, 80 )
{
    this->req_ = move_ptr( req );
    this->callback_ = cb;
}

Session * WebConnector::create_session( )
{
    return new WebRequestSession( this->req_ );
}

void WebConnector::on_new_session( Session * session )
{
    
}

void WebConnector::on_close_session( Session * session )
{
    WebRequestSession* req = scast<WebRequestSession*>( session );

    auto rep = req->response( );

    this->callback_( move_ptr( rep ) );
}

void WebConnector::on_close( )
{

}


















NS_MARATON_END
