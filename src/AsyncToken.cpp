#include "AsyncToken.h"

NS_MARATON_BEGIN

AsyncToken::AsyncToken( callback_t callback )
{
    this->callback_     = callback;
    this->async_.data   = this;
    uv_async_init( uv_default_loop( ) , 
                   &this->async_ , 
                   AsyncToken::uv_async_callback );
}

AsyncToken::~AsyncToken( )
{

}

void AsyncToken::send( void * data )
{
    this->data_ = data;
    uv_async_send( &this->async_ );
}

void AsyncToken::uv_async_callback( uv_async_t * handle )
{
    AsyncToken* token = scast<AsyncToken*>( handle->data );
    if ( token->callback_ != nullptr )
    {
        token->callback_( token , token->data_ );
    }
}

NS_MARATON_END