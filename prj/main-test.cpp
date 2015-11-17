#include "maraton-framework.h"
#include <string>

class MasterFeature :
    public ClusterFeature
{
public:

    MasterFeature( std::string host , int port )
        : ClusterFeature( host , port )
    {
        this->service_->listen( this->host_.c_str() , this->port_ );
    };

    virtual ~MasterFeature()
    {
    };

protected:
     
    virtual void on_new_clusternode( SPTR<ClusterNode> node ) override
    {
      
    }

    virtual void on_close_clusternode( SPTR<ClusterNode> node ) override
    {

    } 

    // Í¨¹ý ClusterFeature ¼Ì³Ð
    virtual SPTR<ClusterNode> create_node( Session * session ) override
    {
        return MAKE_SPTR( ClusterNode , session );
    }
};

int main()
{

    MasterFeature feature( "0.0.0.0" , 90 );

    Server::run();

    return 0;
}