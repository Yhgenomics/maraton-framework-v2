/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-18
* Description:
* * * * * * * * * * * * * * * */

#ifndef ICLUSTER_NODE_H_
#define ICLUSTER_NODE_H_

#include "Session.h"

class Message;

class IClusterNode
{
public:

    IClusterNode                            ( Session* session ) { this->session_ = session; };
    virtual ~IClusterNode                   ( ) { };
    virtual void close                      ( ) = 0;
    virtual void send                       ( UPTR<Message> message ) = 0;

    Session*     session                    ( ){ return this->session_; };
    size_t       id                         ( ){ return this->session_ == nullptr ? 
                                                        0 : 
                                                        this->session_->id();};

private:

    Session*    session_ = nullptr;
};

#endif // !ICLUSTER_NODE_H_
