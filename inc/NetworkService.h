/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-13
* Description:
* * * * * * * * * * * * * * * */

#ifndef SERVICE_H_
#define SERVICE_H_

#include <functional>
#include <vector>

#include "uv.h" 
#include "Define.h"
#include "Session.h"
#include "Utils.h"
#include "EventNotifier.h"

class NetworkService;

class INetworkSubscribe
{
public:

    virtual void evt_service_failed         ( NetworkService* service ,
                                              size_t status ) { };

    virtual void evt_service_open_session   ( NetworkService* service ,
                                              Session* session ) { };

    virtual void evt_service_close_session  ( NetworkService* service ,
                                              Session* session ) { };

    virtual void evt_service_stop           ( NetworkService* service ) { };

};


class NetworkService :
    public EventNotifier<INetworkSubscribe>
{
public: 


    enum ServiceType
    {
        kUnknow = 0 ,
        kServer ,
        kClient
    };

    typedef std::function<void( Session* )>      session_callback_t;
    typedef std::function<void( NetworkService* ,       
                                size_t status )> service_status_callback_t;
    typedef std::function<void( NetworkService* )>      service_callback_t;
   
    NetworkService();
    ~NetworkService();

    NetworkService::ServiceType service_type   ( );
    void listen                                ( const char* host, int port );
    void connect                               ( const char* host, int port );
    void stop                                  ( );

    void data                                  ( void* value ) 
                                               { this->data_ = value; };
    
    void* data                                 ( )
                                               { return this->data_; };

protected:

    virtual Session*        create_session  ( );
    int                     port_                       = 0;
    char                    host_[HOST_NAME_SIZE]       = { 0 };
    char                    ip_[IP_STRING_SIZE]         = { 0 };
    uv_loop_t *             loop            ( );
   
private:
    
    ServiceType             service_type_               = ServiceType::kUnknow;
    std::vector<Session*>   session_list_;

    uv_tcp_t*               uv_tcp_                     = { 0 };
    uv_connect_t*           uv_connect_                 = { 0 };
    uv_getaddrinfo_t*       uv_resolver_                = { 0 };
    addrinfo                addrinfo_                   = { 0 };
    sockaddr_in             addr_in                     = { 0 };
    void*                   data_                       = nullptr;

    //service_status_callback_t      operation_failed_callback_  = nullptr;
    //session_callback_t      new_session_callback_       = nullptr;
    //session_callback_t      close_session_callback_     = nullptr;

    void add_session                        ( Session * session );
    void remove_session                     ( Session * session ); 

    static void uv_callback_alloc_buffer    ( uv_handle_t* handle,
                                              size_t suggested_size,
                                              uv_buf_t* buf );

    static void uv_callback_read            ( uv_stream_t* stream,
                                              ssize_t nread,
                                              const uv_buf_t* buf );
    static void uv_callback_close           ( uv_handle_t* handle );
    static void uv_callback_service_close   ( uv_handle_t* handle );
    static void uv_callback_new_connection  ( uv_stream_t* server, 
                                              int status );
    static void uv_callback_connected       ( uv_connect_t * req, 
                                              int status );
    static void uv_process_resolved         ( uv_getaddrinfo_t* req,
                                              int status,
                                              struct addrinfo* res );


    friend class Session;
};

#endif // SERVICE_H_
