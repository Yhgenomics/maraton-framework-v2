/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-17
* Description:
* * * * * * * * * * * * * * * */

#ifndef ARRAY_H_
#define ARRAY_H_

#include "maraton-framework.h"

template<class T,size_t S>
class Array
{
public:

    void        push( T instance );
    bool        remove( T instance );
    bool        remove( size_t index );
    size_t      size();
    size_t      count();
    T           operator[]( size_t index );

private:

    T                   array_list_[S];
    size_t              element_index_;
};

template<class T , size_t S>
inline void Array<T , S>::push( T instance )
{
    for ( size_t i = 0; i < S; i++ )
    {
        if ( array_list_[i] == nullptr )
        {
            array_list_[i] = instance;
            if ( i > element_index_ )
            {
                element_index = MOVE( i );
            }
            break;
        }
    }
}

template<class T , size_t S>
inline void Array<T , S>::remove( T instance )
{
    for ( size_t i = 0; i <= element_index_; i++ )
    {
        if ( array_list_[i] != nullptr && 
             array_list_[i] == instance )
        {
            array_list_[i] = nullptr;

            return true;
            if ( i == element_index_ )
            {
                --element_index_;
            }
            break;
        }
    }
    return false;
}

template<class T , size_t S>
inline bool Array<T , S>::remove( size_t index )
{
    if ( index >= S )return false;

    array_list_[index] = nullptr;

    if ( index == this->element_index_ )
    {
        this->element_index_--;
    }

    return true;
}

template<class T , size_t S>
inline size_t Array<T , S>::size()
{
    return S;
}

template<class T , size_t S>
inline size_t Array<T , S>::count()
{
    return element_index_ + 1;
}

template<class T , size_t S>
inline T Array<T , S>::operator[]( size_t index )
{
    return MOVE( array_list_[index] );
}

#endif // !ARRAY_H_


