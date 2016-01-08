#include "MRT.h"
#include <thread>
#include <ostream>
#include <string>

using namespace MRT;
using namespace std;

int main( )
{
    MRT::json postJson;
    MRT::json hostConfig;
    string image ="10.0.0.15:5000/bwamem";
    vector<string> enviroment;
    vector<string> binds;

    enviroment.push_back( "t=2" );
    enviroment.push_back( "refgen=hg19.fa" );
    enviroment.push_back( "reads=small" );

    binds.push_back( "/data/input/:/input/" );
    binds.push_back( "/data/output/:/output/" );
    binds.push_back( "/data/ref/:/ref/" );
    binds.push_back( "/dev/shm/:/dev/shm/" );

    postJson[ "Image" ] = image;
    postJson[ "Env" ] = enviroment;
    hostConfig[ "Binds" ] = binds;
    postJson[ "HostConfig" ] = hostConfig;

    MRT::WebClient wc;
    wc.header( "Content-Type" , "application/json" );
    wc.post("http://10.0.0.70:4243/containers/create",  postJson.dump() , []( uptr<MRT::HTTPResponse> rep)
    {
        cout << rep->content()->data() << endl;
        string res = string( rep->content()->data() , rep->content()->size() );
        cout <<res<<endl;

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