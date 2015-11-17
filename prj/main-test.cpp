#include "maraton-framework.h"
//#include <string>
//
//class MasterFeature :
//    public ClusterFeature
//{
//public:
//
//    MasterFeature( std::string host , int port )
//        : ClusterFeature( host , port )
//    {
//        this->service_->listen( this->host_.c_str() , this->port_ );
//    };
//
//    virtual ~MasterFeature()
//    {
//    };
//
//protected:
//     
//    virtual void on_new_clusternode( SPTR<ClusterNode> node ) override
//    {
//        
//    }
//
//    virtual void on_close_clusternode( SPTR<ClusterNode> node ) override
//    {
//        
//    } 
//
//    virtual SPTR<ClusterNode> create_node( Session * session ) override
//    {
//        return MAKE_SPTR( ClusterNode , session );
//    }
//};

int main()
{

    int it = 0;

    it = MOVE( 12234 );

    //MasterFeature feature( "0.0.0.0" , 90 );

    //Server::run();

    return 0;
}