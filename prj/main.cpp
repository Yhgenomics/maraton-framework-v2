#include "MRT.h"
#include <thread>
#include <ostream>
#include <string>
#include <Manager.h>

using namespace MRT;
using namespace std;



class SS : public Session
{
public:

    SS( )
    {
    };

    ~SS( )
    {

    };

protected:

    void on_connect ( )                   override
    {
    };
    void on_read    ( uptr<Buffer> data ) override
    {
    };
    void on_write   ( uptr<Buffer> data ) override
    {
    };
    void on_close   ( )                   override
    {
    };

};


class L : public Listener
{
public:

    L( )
        : Listener( "0.0.0.0" , 1000 )
    {
    };

    ~L( )
    {
    };

protected:

    Session * create_session    ( ) override
    {
        return new SS( );
    };

    void      on_session_open   ( Session * session ) override
    {
    };

    void      on_session_close  ( Session * session ) override
    {
        SAFE_DELETE( session );
    }; 
};

int main( )
{
    /*
        MRT::Manager<MRT::Buffer> mgr;

        mgr.push( make_sptr( MRT::Buffer ) );

        size_t i = SystemInfo::CPUNum( );
        size_t j = SystemInfo::CPUSpeed( 1 );
        size_t m = SystemInfo::MemorySize( );
        size_t f = SystemInfo::MemoryFreeSize( );
        size_t t = SystemInfo::Time( );*/



    while ( true )
    {
        uptr<L> l = make_uptr( L );
        Maraton::instance( )->regist( move_ptr( l ) );
        Maraton::instance( )->run( );
    }


    return 0;
}