// Create by Shubo Yang

#ifndef UTILS_H_
#define UTILS_H_

template<class T>
class Singleton
{

public:

    static T * instance()
    {
        static T * pT = nullptr;
        if ( !pT )
        {
            static T instance;
            pT = &instance;
        }
        return pT;
    }

protected:

    Singleton() {};
    Singleton( const Singleton& rhs ) = delete;
    Singleton& operator=( const Singleton& rhs ) = delete;

};

#endif // UTILS_H_
