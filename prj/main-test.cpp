#include "maraton.h"
#include "maraton-framework.h"
#include "MasterBaseFeature.h"

int main()
{
    
    MasterBaseFeature feature( "0.0.0.0" , 4455 );

    Server::run();

    return 0;
}