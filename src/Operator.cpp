#include "Operator.h"
#include "Maraton.h"

NS_MARATON_BEGIN

void Operator::stop( )
{
    uv_close( ( uv_handle_t* ) &this->uv_tcp_ ,
              Operator::uv_callback_close );
}

Session * Operator::create_session( )
{        
    return new Session( );
}

void Operator::uv_callback_connected( uv_connect_t * req ,
                                      int status )
{
    int result = 0;
    Operator * opt = scast < Operator* > ( req->data );

    if ( status != 0 )
    {
        LOG_DEBUG_UV( status );

        opt->stop( );

        return;
    }

    if ( opt == nullptr )
    {
        LOG_DEBUG( "operator is nullptr" );
        return;
    }

    LOG_DEBUG( "%s:%d connected" , opt->address_.c_str( ) , opt->port_ );

    auto session = opt->create_session( );
    session->uv_on_connected( opt );
    opt->on_new_session( session );

    LOG_DEBUG_UV( result );
}

void Operator::uv_callback_new_connection( uv_stream_t * server ,
                                           int status )
{
    int result = 0;
    auto opt = scast<Operator*>( server->data );

    if ( status < 0 )
    {
        LOG_DEBUG_UV( status );
        return;
    }

    if ( opt == nullptr )
    {
        LOG_DEBUG( "operator is nullptr!" );
        return;
    }

    auto session = opt->create_session( );

    if ( session == nullptr )
    {
        LOG_DEBUG( "can not create session!" );
        return;
    }

    result = uv_tcp_init( opt->uv_loop_ , session->uv_tcp_ );
    LOG_DEBUG_UV( result );

    result = uv_accept( server , ( uv_stream_t* ) session->uv_tcp_ );
    LOG_DEBUG_UV( result );

    if ( result == 0 )
    {
        session->uv_on_accepted ( opt );
        opt->on_new_session     ( session );
    }
    else
    {
        session->uv_on_close    ( );
        opt->on_close_session   ( session );
    }
}

void Operator::uv_callback_close( uv_handle_t * handle )
{
    int result = 0;
    auto opt = scast<Operator*>( handle->data );

    if ( opt == nullptr )
    {
        LOG_DEBUG( "operator is nullptr!" );
        return;
    }

    opt->on_close( );
    Maraton::instance( )->unregist( opt );
}

NS_MARATON_END