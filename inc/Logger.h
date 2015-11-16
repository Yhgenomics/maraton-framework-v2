/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef LOGER_H_
#define LOGER_H_ 

#include "stdio.h"
#include <stdarg.h>

class Logger
{
public:

    template<typename ...Types>
    static void sys( const char* fmt, Types... args)
    {

#ifdef _WIN32
        printf( "[SYS] " );
#else
        printf( "\033[1;33m[SYS] " );
#endif

        printf( fmt, args... );

#ifdef _WIN32
        printf( "\r\n" );
#else
        printf( "\033[0m\r\n" );
#endif      

    }

    template<typename ...Types>
    static void error( const char* fmt, Types... args )
    {
#ifdef _WIN32
        printf( "[ERROR] " );
#else
        printf( "\033[0;31m[ERROR] " );
#endif

        printf( fmt , args... );

#ifdef _WIN32
        printf( "\r\n" );
#else
        printf( "\033[0;31m\r\n" );
#endif    
    }

    template<typename ...Types>
    static void log( const char* fmt, Types... args )
    { 
        printf( fmt , args... );
        printf( "\r\n" );
    }
private:

    Logger() {};
    ~Logger() {};
};

#endif // !LOGER_H_ 