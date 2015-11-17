#include "HTTPFeature.h"

HTTPFeature * HTTPFeature::instance()
{
    static HTTPFeature* ret = nullptr;
    if ( ret == nullptr )
    {
        static HTTPFeature ret_inst;
        ret = &ret_inst;
    }
    return ret;
}

void HTTPFeature::send_request( UPTR<HTTPRequest> req , callback_http_response_t callback)
{
    auto token      = MAKE_SPTR( HTTPFeatureToken );
    token->service  = MAKE_UPTR( NetworkService );

    token->service->register_notifier( this );
    token->service->data( token.get() );

    token->callback_rep = callback;
    token->req          = MOVE( req );

    this->service_array_.push( MOVE(token) );
}

void HTTPFeature::evt_service_failed( NetworkService * service , size_t status )
{
    for ( size_t i = 0; i < this->service_array_.count(); i++ )
    {
        if ( this->service_array_[i] != nullptr && 
             this->service_array_[i].get()->service.get() == service )
        {
            this->service_array_.remove( i );
        }
    }
}

void HTTPFeature::evt_service_open_session( NetworkService * service , Session * session )
{
    HTTPFeatureToken* token = static_cast<HTTPFeatureToken*>(service->data());

}

void HTTPFeature::evt_service_close_session( NetworkService * service , Session * session )
{
}

void HTTPFeature::evt_service_stop( NetworkService * service )
{
    for ( size_t i = 0; i < this->service_array_.count(); i++ )
    {
        if ( this->service_array_[i] != nullptr && 
             this->service_array_[i].get()->service.get() == service )
        {
            this->service_array_.remove( i );
        }
    }
}
