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

protected:

    virtual void        do_work          ( ) override;
    virtual void        on_new_session   ( Session * session ) = 0;
    virtual void        on_close_session ( Session * session ) = 0;
    virtual void        on_close         ( ) = 0;
};

NS_MARATON_END

#endif // !MRT_CONNECTOR_H_
