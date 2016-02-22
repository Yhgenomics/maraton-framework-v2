#include "Maraton.h"
#include "uv.h"

NS_MARATON_BEGIN

void Maraton::regist( sptr<Operator> opt )
{
    opt->addrinfo_.ai_family       = PF_INET;
    opt->addrinfo_.ai_socktype     = SOCK_STREAM;
    opt->addrinfo_.ai_protocol     = IPPROTO_TCP;
    opt->addrinfo_.ai_flags        = 0;
    opt->uv_getaddrinfo_.data      = opt.get( );
    opt->uv_loop_                  = this->uv_loop( );

    int r = uv_getaddrinfo( opt->uv_loop_ ,
                            &opt->uv_getaddrinfo_ ,
                            Maraton::uv_process_resolved ,
                            opt->address_.c_str( ) ,
                            NULL ,
                            &opt->addrinfo_ );

    opt->index_             = elements_index_;
    elements_[opt->index_]  = sptr<Operator>( opt );
    elements_index_         = ( elements_index_ + 1 ) % MAX_CONNECTION_SIZE;
}

void Maraton::uv_process_resolved( uv_getaddrinfo_t * req , int status , addrinfo * res )
{
    int result      = 0;
    Operator * opt  = scast<Operator*>( req->data );

    if ( opt == nullptr )
    {
        LOG_DEBUG( "operator is nullptr" );
        return;
    } 

    if( status < 0 )
    {
        LOG_DEBUG_UV( status );
        Maraton::instance( )->unregist( opt );
        delete res;
        return;
    }

    char ip[17] = { 0 };

    uv_ip4_name( ( struct sockaddr_in* ) res->ai_addr , 
                 ip , 
                 16 );
    uv_ip4_addr( ip , 
                 opt->port_ , 
                 &opt->addr_in_ );
    opt->ip_ = ip;

    opt->do_work();

    req->data = nullptr;
    delete res;
}

void Maraton::unregist( sptr<Operator> opt )
{
    this->unregist( opt.get( ) );
}

void Maraton::run( )
{
    uv_run( this->uv_loop( ) , UV_RUN_DEFAULT );
}

void Maraton::unregist( const Operator * opt )
{
    if ( opt == nullptr )
        return;

    elements_[opt->index_] = nullptr;
}

uv_loop_t * Maraton::uv_loop( )
{
    return uv_default_loop( );
}

NS_MARATON_END