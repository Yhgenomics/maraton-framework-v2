#include "SystemInfo.h"
#include "Timer.h"

size_t Timer::tick()
{
    return MRT::SystemInfo::Time( );
};
