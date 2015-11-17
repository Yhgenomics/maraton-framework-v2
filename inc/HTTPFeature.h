/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-17
* Description:
* * * * * * * * * * * * * * * */

#ifndef HTTP_FEATURE_H_
#define HTTP_FEATURE_H_

#include <functional>

#include "maraton-framework.h"

typedef std::function<void( UPTR<HTTPResponse> )> callback_http_response_t;
typedef std::function<void( UPTR<HTTPRequest> )> callback_http_request_t;


class HTTPResponse
{

};

class HTTPRequest
{

};

class HTTPFeatureToken
{
public:

    UPTR<HTTPRequest>           req;
    UPTR<HTTPResponse>          rep;
    UPTR<NetworkService>        service;
    callback_http_response_t    callback_rep;
    callback_http_request_t     callback_req;
};

class HTTPFeature :
    public Feature
{
public:


    static HTTPFeature* instance();

    void                send_request               ( UPTR<HTTPRequest> req , 
                                                     callback_http_response_t callback);

    void                evt_service_failed         ( NetworkService* service ,
                                                     size_t status );

    void                evt_service_open_session   ( NetworkService* service ,
                                                     Session* session );

    void                evt_service_close_session  ( NetworkService* service ,
                                                     Session* session ) ;

    void                evt_service_stop           ( NetworkService* service );

private:

    Array<SPTR<HTTPFeatureToken> , 1024> service_array_;

};

#endif // !HTTP_FEATURE_H_
