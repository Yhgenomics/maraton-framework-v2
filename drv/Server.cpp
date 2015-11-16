#include "Server.h"
#include "uv.h"

void Server::add_feature( Feature * feature )
{

}

void Server::run()
{
    while ( true )
    {
        uv_run( uv_default_loop() , UV_RUN_DEFAULT );
    }
}
