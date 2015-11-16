#ifndef DEFINE_H_
#define DEFINE_H_

#include <memory>
#include "Logger.h"
// Switchers
// Controls debug mode
#define DEBUG_MODE 1

// Cast definitions
#define CIRCLE_BUFFER_SIZE          1024 * 1024
#define SESSION_RECIVE_BUFFER_SIZE  1024 * 128
#define HOST_NAME_SIZE              512
#define IP_STRING_SIZE              16

// Macro function definitions
#define PP_DEF(__type__,__name__)                           \
    void __name__(__type__ value){ __name__##_ = value; };  \
    __type__ __name__(){ return __name__##_; };

#define SAFE_DELETE(__x__)                                  \
    if ( nullptr != __x__ )                                 \
    {                                                       \
        delete __x__;                                       \
        __x__ = nullptr;                                    \
    }

#define SAFE_DELETE_ARRAY(__x__)                            \
    if ( nullptr != __x__ )                                 \
    {                                                       \
        delete[] __x__;                                     \
        __x__ = nullptr;                                    \
    }

#define SINGLETON_BEGIN(__class__)                          \
class __class__                                             \
{                                                           \
private:                                                    \
    __class__(){};                                          \
    ~__class__(){};                                         \
public:                                                     \
    static __class__ * instance()                           \
    {                                                       \
        static __class__ * pT = nullptr;                    \
        if ( !pT )                                          \
        {                                                   \
            static __class__ instance;                      \
            pT = &instance;                                 \
        }                                                   \
        return pT;                                          \
    };

#define SINGLETON_END };


#define LOG_UV_ERROR(__x__) if ( __x__ != 0 ) printf( "uv error: %s", \
                                                  uv_strerror((int)__x__));
// C11 definitions
#define UPTR std::unique_ptr
#define SPTR std::shared_ptr
#define WPTR std::weak_ptr
#define DEF_UPTR(__t__,__v__,...)  std::unique_ptr< __t__ > __v__ = \
                                   std::unique_ptr< __t__ >( new __t__(__VA_ARGS__) )
                                   
#define DEF_SPTR(__t__,__v__,...)  std::shared_ptr< __t__ > __v__ =  \
                                   std::make_shared< __t__ >(__VA_ARGS__)

#define MAKE_UPTR(__t__,...) std::unique_ptr< __t__ >( new __t__(__VA_ARGS__) )
#define MAKE_SPTR(__t__,...) std::make_shared< __t__ >(__VA_ARGS__)
#define MOVE(__T__) std::move(__T__)

// Debug mode definitions
#if DEBUG_MODE
#define LOG_DEBUG(msg_,...) Logger::log("%s:%d "##msg_,__FILE__,__LINE__,##__VA_ARGS__)
#define LOG_DEBUG_UV(status) if( status != 0 ) Logger::log("%s:%d uv error: %s",__FILE__,__LINE__,uv_strerror((int)status))
#else
#define LOG_DEBUG(msg_,...) 
#define LOG_DEBUG_UV(status)
#endif

// Message definitions
#define LOG_SYS(msg_,...) Logger::sys(msg_,__VA_ARGS__)
#define LOG_EERROR(msg_,...) Logger::error(msg_,__VA_ARGS__)
#define UV_ERROR(status) uv_strerror((int)status)

#endif // !DEFINE_H_
