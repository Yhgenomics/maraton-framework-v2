/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-16
* Description:
* * * * * * * * * * * * * * * */

#ifndef MASTER_BASE_FEATURE_H_
#define MASTER_BASE_FEATURE_H_

#include <string>

#include "ClusterFeature.h"

class MasterBaseFeature :
    public ClusterFeature
{
public:

    MasterBaseFeature( std::string host );
    virtual ~MasterBaseFeature();  
};

#endif // !MASTER_BASE_FEATURE_H_
