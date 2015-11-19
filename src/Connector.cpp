#include "Connector.h"

NS_MARATON_BEGIN

Connector::Connector( std::string addr , int port )
    : Operator( addr , port )
{

}

Connector::~Connector()
{

}

void Connector::do_work()
{
    uv_tcp_init( this->uv_loop_ , 
                 &this->uv_tcp_ );

    //Session* session = this->create_session( );

    auto result = uv_tcp_connect( &this->uv_connect_ , 
                                  &this->uv_tcp_ ,
                                  ( const struct sockaddr* ) &this->addr_in ,
                                  Operator::uv_callback_connected );

    LOG_DEBUG_UV( result );
}
 
void Connector::on_close_session( Session * session )
{
    this->stop();
}

NS_MARATON_END
