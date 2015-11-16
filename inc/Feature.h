/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-16
* Description:
* * * * * * * * * * * * * * * */

#ifndef FEATURE_H_
#define FEATURE_H_

#include "NetworkService.h"

class Feature :
    public INetworkSubscribe
{
public:

    Feature();
    virtual ~Feature();

protected: 

    UPTR<NetworkService> service_ = nullptr;

private:

    friend class Server;
};

#endif // !FEATURE_H_
