#include "Buffer.h"

NS_MARATON_BEGIN

Buffer::Buffer( )
{
    this->data_  = nullptr;
    this->pdata_ = this->data_;
    this->size_  = 0;
}

Buffer::Buffer(size_t size)
{
    SAFE_DELETE( this->data_ );

    this->data_  = new char[size] { 0 };
    this->pdata_ = this->data_;
    this->size_  = size;
}

Buffer::Buffer( std::string string )
{
   this->data( string.c_str() , string.size() );
}

Buffer::Buffer( const char * data , size_t size )
{
    this->data( data , size );
}

Buffer & Buffer::operator=( Buffer & buffer )
{
    this->data( buffer.data_ , buffer.size_ );
    return *this;
}

Buffer & Buffer::operator=( Buffer && buffer )
{
    this->data_  = buffer.data_;
    this->size_  = buffer.size_;
    this->pdata_ = this->data_;

    buffer.data_ = nullptr;
    buffer.size_ = 0;

    return *this;
}

Buffer & Buffer::operator+=( const Buffer & buffer )
{
    size_t total_len   = buffer.size_ + this->size_;
    char* tmp          = new char[total_len] { 0 };
    memcpy( tmp , this->data_ , this->size_ );
    SAFE_DELETE( this->data_ );
    memcpy( tmp + this->size_ , buffer.data_, buffer.size_ );
    this->data_ = tmp;
    this->size_ = total_len;
    return *this;
}

char Buffer::operator[]( const size_t index )
{
    if ( this->data_ == nullptr )
    {
        throw "Buffer.data is nullptr";
    }

    if ( this->size_ <= index )
    {
        throw "Buffer out of size!!!";
    }

    return *( this->data_ + index );
}

Buffer::~Buffer()
{
    SAFE_DELETE( this->data_ );
}

Buffer::Buffer( Buffer & buffer )
{
    this->data( buffer.data_ , buffer.size_ );
}

Buffer::Buffer( Buffer && buffer )
{
    this->data_  = buffer.data_;
    this->size_  = buffer.size_;
    this->pdata_ = this->data_;

    buffer.data_ = nullptr;
    buffer.size_ = 0;
}

char * Buffer::data()
{
    return this->data_;
}

void Buffer::data( const char * value , size_t size )
{
    SAFE_DELETE( this->data_ );

    this->size_ = size;
  
    if( value == nullptr )
    {
        return;
    }

    this->data_     = new char[size] { 0 };
    this->pdata_    = this->data_;
    memcpy( this->data_ , value , size );
}

void Buffer::push( const char * data , size_t len )
{
    if ( this->data_ == nullptr )
        return;

    size_t delta = this->size_ - ( size_t )( this->pdata_ - this->data_ );

    if ( delta > len )
    {
        memcpy( this->pdata_ , data , len );
    }
    else
    {
        memcpy( this->pdata_ , data , delta );
    }

    this->pdata_+=delta;
}

void Buffer::clearup()
{
    SAFE_DELETE( this->data_ );
    this->size_ = 0;
}

NS_MARATON_END