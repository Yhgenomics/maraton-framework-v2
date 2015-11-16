/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-16
* Description:
* * * * * * * * * * * * * * * */

#ifndef SERVER_H_
#define SERVER_H_

#include "Feature.h"

class Server
{
public:

    static void add_feature(Feature * feature);
    static void run();
};

#endif // !SERVER_H_
