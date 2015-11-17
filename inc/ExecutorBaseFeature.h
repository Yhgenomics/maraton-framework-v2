/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-16
* Description:
* * * * * * * * * * * * * * * */

#ifndef EXECUTOR_BASE_FEATURE_H_
#define EXECUTOR_BASE_FEATURE_H_

#include <string>

#include "ClusterFeature.h"

class ExecutorBaseFeature :
    public ClusterFeature
{
public:

    ExecutorBaseFeature                     ( std::string host );
    ~ExecutorBaseFeature                    ( );  
};

#endif // !EXECUTOR_BASE_FEATURE_H_
