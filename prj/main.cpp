#include "MRT.h"

using namespace MRT;

class TSession
    : public Session
{
protected:

    virtual void on_read( uptr<Buffer> data ) override
    {
        this->send( move_ptr( data ) );
    }

    virtual void on_connect( ) override
    {

    }

    virtual void on_close( ) override
    {

    }

    virtual void on_write( uptr<Buffer> data ) override
    {
        LOG_DEBUG( "writes : %lld" , data->size( ) );
    }
};

class TConnector :
    public Connector
{
public:

    TConnector( std::string address , int port )
        : Connector( address , port )
    {

    }

protected:

    virtual Session* create_session( ) override
    {
        return new TSession( );
    };

    virtual void on_new_session( Session * session ) override
    {

    }
    virtual void on_close_session( Session * session ) override
    {

    }
    virtual void on_close( ) override
    {

    }
};

class TListener
    : public Listener
{
public:

    TListener( std::string address , int port )
        : Listener( address , port )
    {

    }

protected:

    virtual Session* create_session( ) override
    {
        return new TSession( );
    };

    virtual void on_new_session( Session * session ) override
    {

    }

    virtual void on_close_session( Session * session ) override
    {

    }

    virtual void on_close( ) override
    {

    }
};

void test_listen( )
{
    Maraton::instance( )->regist( make_uptr( TListener , "localhost" , 4455 ) );
}

void test_connect( )
{
    for ( size_t i = 0; i < 1000; i++ )
    {
        Maraton::instance( )->regist( make_uptr( TConnector , "localhost" , 4455 ) );
    }
}

int main( )
{
    while ( 1 )
    {
        sptr<HTTPRequest> req = make_sptr( HTTPRequest , "/" , "GET" );

        //uptr<WebConnector> conn = make_uptr( WebConnector , "www.baidu.com" , req , [ ] ( uptr<HTTPResponse> rep )
        //{ 
        //    auto p = rep;
        //    printf( "Callback %d " ,   );
        //} );
        //Maraton::instance( )->regist( move_ptr( conn ) );

        //test_listen( );
        //test_connect( );
        Maraton::instance( )->loop( );
    }

    return 0;
}