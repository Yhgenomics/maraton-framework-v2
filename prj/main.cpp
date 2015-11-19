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

    virtual Session * create_session( ) override
    {
        auto ret = new TSession( );
        return scast<Session*>( ret );
    };

    virtual void on_close_session( Session * session ) override
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

    virtual Session * create_session( ) override
    {
        auto ret = new TSession( );
        return scast<MRT::Session*>( ret );
    };

    virtual void on_close_session( Session * session ) override
    {

    }
};

int main( )
{
    while ( 1 )
    {
        for ( size_t i = 0; i < 1000; i++ )
        {
            Maraton::instance( )->regist( make_uptr( TConnector , "10.0.0.10" , 4455 ) );
        }
        Maraton::instance( )->loop( );
    }

    return 0;
}