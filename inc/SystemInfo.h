/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2016-02-22
* Description: System information helper 
               class
* * * * * * * * * * * * * * * */

#ifndef SYSETEM_INFO_H_
#define SYSETEM_INFO_H_

#include <string>
#include <Macro.h>
#include <uv.h>

NS_MARATON_BEGIN

class SystemInfo
{
public:

    static size_t MemorySize( )
    {
        return uv_get_total_memory( );
    };

    static size_t MemoryFreeSize( )
    {
        return uv_get_free_memory( );
    };

    static int CPUNum( )
    {
        int cpu_count            = 0;
        uv_cpu_info_t * cpu_info = { 0 };

        uv_cpu_info     ( &cpu_info , &cpu_count );
        uv_free_cpu_info( cpu_info  , cpu_count );
        
        return cpu_count;
    };

    static int CPUSpeed( int id )
    {
        int cpu_speed            = 0;
        int cpu_count            = 0;
        int result               = 0;
        uv_cpu_info_t * cpu_info = { 0 };

        result = uv_cpu_info( &cpu_info , &cpu_count );

        if ( result < 0 )
        {
            LOG_DEBUG_UV( result );
            return result;
        }

        if ( id >= cpu_count )
            return -1;

        cpu_speed = cpu_info[id].speed;

        uv_free_cpu_info( cpu_info , cpu_count );

        return cpu_speed;
    };

    static std::string CurrentDirectory( )
    {
        char    path[1024] = { 0 };
        size_t  path_len   = 0;

        uv_cwd( path , &path_len );

        return std::string( path , path_len );
    };

    static size_t Time( )
    {
        return ( uv_hrtime( ) / 1000 );
    };
};

NS_MARATON_END

#endif // !SYSETEM_INFO_H_
