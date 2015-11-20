/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-19
* Description:
* * * * * * * * * * * * * * * */

#ifndef MRT_LISTENER_H_
#define MRT_LISTENER_H_

#include <string>

#include "Macro.h"
#include "Operator.h"

NS_MARATON_BEGIN

class Listener :
    public Operator
{
public:

    Listener    ( std::string addr , 
                  int port );
    ~Listener   ( );

protected:

    virtual void        do_work         ( ) override;

    virtual void        on_new_session  ( Session * session )   = 0;
    virtual void        on_close_session( Session * session ) = 0;
    virtual void        on_close        ( ) = 0;
};

NS_MARATON_END

#endif // !MRT_LISTENER_H_
