/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-13
* Description:
* * * * * * * * * * * * * * * */

#ifndef EVENT_NOTIFIER_H_
#define EVENT_NOTIFIER_H_

#define SAFE_NOTIFY(__notifier__,__func__,...) if ( __notifier__ != nullptr ) __notifier__->__func__( __VA_ARGS__ )

template<class T>
class EventNotifier
{ 
public:

    virtual void register_notifier( T* notifier );

protected:

    T* notifier_ = nullptr;
};


template<class T>
inline void EventNotifier<T>::register_notifier( T* notifier )
{
    this->notifier_ = notifier;
}

#endif // !EVENT_NOTIFIER_H_
