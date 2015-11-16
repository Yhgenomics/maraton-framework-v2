/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-11
* Description:
* * * * * * * * * * * * * * * */

#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <vector>
#include "Define.h"

template<typename T>
class SessionManager 
{
public:

    static SessionManager<T>* instance()
    {
        static SessionManager<T> * pT = nullptr;
        static SessionManager<T> instance;
        if ( !pT )
        {
            pT = &instance;
        }
        return pT;
    };

    bool push( T* element )
    {
        this->elements_.push_back( element );
        return true;
    };

    bool remove( T* element )
    {
        for ( auto i = this->elements_.begin(); i != this->elements_.end(); i++ )
        {
            if ( ( *i ) == element ) 
            {
                this->elements_.erase( i );
                return true;
            }
        }
        return false;
    };

    bool exist( T* element )
    {
        for ( auto i = this->elements_.begin(); i != this->elements_.end(); i++ )
        {
            if ( ( *i ) == element )
            {
                return true;
            }
        }
        return false;
    };

private:

    SessionManager<T>() {};
    ~SessionManager<T>() {};
    std::vector<T*> elements_;
};

#endif // !SESSION_MANAGER_H_
