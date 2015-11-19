#include "Maraton.h"

NS_MARATON_BEGIN

void Maraton::regist( uptr<Operator> listener )
{
    listener->addrinfo_.ai_family       = PF_INET;
    listener->addrinfo_.ai_socktype     = SOCK_STREAM;
    listener->addrinfo_.ai_protocol     = IPPROTO_TCP;
    listener->addrinfo_.ai_flags        = 0;
    listener->uv_getaddrinfo_.data      = listener.get( );
    listener->uv_loop_                  = this->uv_loop( );

    int r = uv_getaddrinfo( listener->uv_loop_ ,
                            &listener->uv_getaddrinfo_ ,
                            Maraton::uv_process_resolved ,
                            listener->address_.c_str( ) ,
                            NULL ,
                            &listener->addrinfo_ );

    elemnts_.push_back( move_ptr( listener ) );
}

void Maraton::uv_process_resolved( uv_getaddrinfo_t * req , int status , addrinfo * res )
{
    int result = 0;
    Operator * opt = scast<Operator*>( req->data );

    if ( opt == nullptr )
    {
        LOG_DEBUG( "operator is nullptr" );
        return;
    } 

    char ip[17] = { 0 };

    uv_ip4_name( ( struct sockaddr_in* ) res->ai_addr , 
                 ip , 
                 16 );
    uv_ip4_addr( ip , 
                 opt->port_ , 
                 &opt->addr_in );
    opt->ip_ = ip;
    opt->do_work();
}

void Maraton::unregist( const Operator * opt )
{
    for ( auto itr=elemnts_.begin( ); itr != elemnts_.end( ); itr++ )
    {
        if ( ( *itr ).get( ) == opt )
        {
            elemnts_.erase( itr );
            break;
        }
    }
}

void Maraton::loop( )
{
    uv_run( this->uv_loop( ) , UV_RUN_DEFAULT );
}

uv_loop_t * Maraton::uv_loop( )
{
    return uv_default_loop( );
}

NS_MARATON_END