#include "..\inc\Timer.h"

size_t Timer::tick()
{
    clock_t t = clock();
    return static_cast< size_t >( t );
};
