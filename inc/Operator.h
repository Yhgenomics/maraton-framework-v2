/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-19
* Description:
* * * * * * * * * * * * * * * */

#ifndef MRT_OPERATOR_H_
#define MRT_OPERATOR_H_

#include <string>

#include "Macro.h"
#include "uv.h"
#include "Session.h"

NS_MARATON_BEGIN

class Operator
{
public:

    Operator(std::string address,int port)
    {
        this->address_              = address;
        this->port_                 = port;
        this->uv_tcp_.data          = this;
        this->uv_getaddrinfo_.data  = this;
        this->uv_connect_.data      = this;
    }

    virtual ~Operator( )
    {

    }

    virtual void stop();

protected:

    virtual void        do_work( )                           = 0;
    virtual void        on_new_session( Session* session )   = 0;
    virtual void        on_close_session( Session* session ) = 0;
    virtual Session*    create_session( )                    = 0;
    virtual void        on_close( )                          = 0;

    int                 index_                               = 0;
    uv_tcp_t            uv_tcp_                              = { 0 };
    uv_connect_t        uv_connect_                          = { 0 };
    uv_getaddrinfo_t    uv_getaddrinfo_                      = { 0 };
    uv_loop_t*          uv_loop_                             = nullptr;
                                                             
    addrinfo            addrinfo_                            = { 0 };
    sockaddr_in         addr_in                              = { 0 };
    std::string         address_                             = "";
    std::string         ip_                                  = "";
    int                 port_                                = 0; 

    static void uv_callback_connected      ( uv_connect_t * req , 
                                             int status );

    static void uv_callback_new_connection ( uv_stream_t * server , 
                                             int status );

    static void uv_callback_close          ( uv_handle_t * handle ); 

private:

    friend class Session;  
    friend class Maraton;
};

NS_MARATON_END

#endif // !MRT_OPERATOR_H_
