/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef MRT_LOGER_H_
#define MRT_LOGER_H_ 

#include "stdio.h"
#include <stdarg.h>
#include "Timer.h"

class Logger
{
public:

    template<typename ...Types>
    static void sys( const char* fmt , Types... args )
    {

#ifdef _WIN32
        printf( "[SYS] %lld: " , Timer::tick( ) );
#else
        printf( "\033[1;33m[SYS] %zu: " , Timer::tick( ) );
#endif

        printf( fmt , args... );

#ifdef _WIN32
        printf( "\r\n" );
#else
        printf( "\033[0m\r\n" );
#endif      

    }

    template<typename ...Types>
    static void error( const char* fmt , Types... args )
    {
#ifdef _WIN32
        printf( "[ERROR] %lld: " , Timer::tick( ) );
#else
        printf( "\033[0;31m[ERROR] %zu: " , Timer::tick( ) );
#endif

        printf( fmt , args... );

#ifdef _WIN32
        printf( "\r\n" );
#else
        printf( "\033[0;31m\r\n" );
#endif    
    }

    template<typename ...Types>
    static void log( const char* fmt , Types... args )
    {
#ifdef _WIN32
        printf( "%lld: " , Timer::tick( ) );
#else
        printf( "%zu: " , Timer::tick( ) );
#endif   
        printf( fmt , args... );
        printf( "\r\n" );
    }
private:

    Logger( )
    {
    };
    ~Logger( )
    {
    };
};

#endif // !MRT_LOGER_H_ 