#include "Listener.h"

NS_MARATON_BEGIN 
 
Listener::Listener( std::string addr , int port )
    : Operator( addr , port )
{

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
                          ( const struct sockaddr* )&this->addr_in ,
                          0 );
    LOG_DEBUG_UV( result );

    result = uv_listen  ( ( uv_stream_t* ) &this->uv_tcp_ ,
                          0 ,
                          Operator::uv_callback_new_connection );
    LOG_DEBUG_UV( result );
}
void Listener::on_new_session( Session * session )
{
}
void Listener::on_close_session( Session * session )
{
}
Session * Listener::create_session()
{
    return nullptr;
}
void Listener::on_close()
{
}
NS_MARATON_END
