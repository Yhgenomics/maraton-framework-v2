/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-10-22
* Description:
* * * * * * * * * * * * * * * */

#ifndef NETWORK_DATA_BUILDER_H_
#define NETWORK_DATA_BUILDER_H_

#include <memory.h>
#include <Define.h>
#include "Buffer.h"
 
class CircleBuffer
{
public:

    CircleBuffer();
    
    ~CircleBuffer();

    virtual bool push           ( Buffer & buf );

    virtual UPTR<Buffer> pop    ( size_t len );

    size_t length               ( ) { return this->buffer_length_; };
    size_t used_length          ( ) { return this->used_len_; };
    size_t available_length     ( ) { return this->available_len_; };

    size_t tail_ = 0;
    size_t head_ = 0;

private:

    CircleBuffer                ( CircleBuffer & ) = delete;
    CircleBuffer                ( CircleBuffer && ) = delete;
    CircleBuffer operator=      ( CircleBuffer ) = delete;

    size_t used_len_            = 0;
    size_t available_len_       = 0;

    const int buffer_length_    = CIRCLE_BUFFER_SIZE;

    char* circle_buffer_        = nullptr;
};

#endif // !NETWORK_DATA_BUILDER_H_