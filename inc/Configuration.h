/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       :
* Description:
* * * * * * * * * * * * * * * */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_ 

#include "Define.h"
#include "json.hpp"
#include <string>

class Configuration
{
public:

    static Configuration * instance()          
    {                                      
        static Configuration * pT = nullptr;   
        if ( !pT )                         
        {                                  
            static Configuration instance;     
            pT = &instance;                
        }                                  
        return pT;                         
    };

    std::string get( std::string key );

protected:

    Configuration();

private:

    nlohmann::json config;

    static const int MAX_PATH_LEN = 512;

};

#endif //!CONFIGURATION_H_ 