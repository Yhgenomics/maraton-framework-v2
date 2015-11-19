#include "Configuration.h"
#include <stdio.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>
#endif

NS_MARATON_BEGIN

std::string Configuration::get( std::string key )
{
    return this->config[key];
}

Configuration::Configuration( )
{
    char path[MAX_PATH_LEN] = { 0 };

#ifdef _WIN32
    _getcwd( path , MAX_PATH_LEN );
#else
    getcwd( path , MAX_PATH_LEN );
#endif

    FILE* file;

#ifdef _WIN32
    fopen_s( &file, "maraton.conf" , "r" );
#else
    file = fopen( "maraton.conf" , "r" );
#endif

    if ( file == NULL )
    {
        LOG_DEBUG( "can not open maraton.conf" );
        return;
    }

    LOG_DEBUG( "successfully open maraton.conf" );
 
    fseek( file , 0 , SEEK_END );
    int len = ftell( file );
    fseek( file , 0 , SEEK_SET );

    uptr<char> json_string( new char[len + 1] );
    memset( json_string.get() , 0 , len + 1 );
    fread ( json_string.get() , 1 , len , file );
    fclose( file );

    this->config = json::parse( json_string.get() );
}

NS_MARATON_END