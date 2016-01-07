#include "MRT.h"
#include <thread>

using namespace MRT;


int main( )
{
    MRT::WebClient wc;
    
    wc.post("http://10.0.0.70/containers/609f1144a8da8305c21ff/start", "" , []( uptr<MRT::HTTPResponse> rep)
    {
        printf( "done\r\n" );
    } );

    while(true)
    {
        MRT::Maraton::instance( )->loop( );
    }
    return 0;
   /* while ( 1 )
    {
        for ( size_t i = 0; i < 1; i++ )
        {
            WebClient client;
            client.get( "http://www.verycd.com:123123" , [ ] ( uptr<HTTPResponse> rep )
            {
                if ( rep == nullptr ) return;
                LOG_DEBUG( "HTTP Response[%lld] %lld bytes" , rep->status( ) , rep->content_length( ) );
                LOG_DEBUG( "%s" , rep->content( )->data( ) );
            } );
        }

        Maraton::instance( )->loop( );
        
        std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
    }*/
    return 0;
}