/* * * * * * * * * * * * * * * *
* YHGenomics Inc.
* Author     : yang shubo
* Date       : 2015-11-19
* Description:
* * * * * * * * * * * * * * * */

#ifndef MRT_CONNECTOR_H_
#define MRT_CONNECTOR_H_

#include <string>

#include "Macro.h"
#include "Operator.h"

NS_MARATON_BEGIN

class Connector :
    public Operator
{
public:

    Connector           ( std::string addr,
                          int port );
    ~Connector          ( );

    virtual void do_work( ) override;

protected:

    virtual void        on_new_session    ( Session * session ) override;
    virtual void        on_close_session  ( Session * session ) override;
    virtual Session *   create_session    ( ) override;
    virtual void        on_close          ( ) override;
};

NS_MARATON_END

#endif // !MRT_CONNECTOR_H_
