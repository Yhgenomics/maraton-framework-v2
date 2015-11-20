#include "MRT.h"

using namespace MRT;

int main( )
{
    while ( 1 )
    {
        //Maraton::instance( )->regist( make_uptr( Listener , "localhost" , 1113 ) );
        //Maraton::instance( )->regist( make_uptr( Connector , "localhost" , 1113 ) );
        Maraton::instance( )->loop( );
    }
    return 0;
}