#include "ClusterNode.h"

ClusterNode::ClusterNode( Session * session )
{
    this->session_ = session;
    this->session_->register_notifier( this );
}

ClusterNode::~ClusterNode()
{

}

void ClusterNode::close()
{
    this->session_->close();
}

void ClusterNode::send( UPTR<Message> message )
{ 
    Buffer header( "YH" );
    Buffer length( sizeof( short ) * 2 );
    Buffer body  =  message->bytes();

    short* pdata = (short*)( length.data() );
    *pdata       = (short)body.size();
    ++pdata;
    *pdata       = (short)body.size();

    this->session_->send( header );
    this->session_->send( length );
    this->session_->send( body );
}

void ClusterNode::evt_session_close( Session * session)
{
    if ( this->session() != session )
    {
        return;
    }

    if ( this->notifier_ != nullptr )
    {
        this->notifier_->evt_clustersession_close( this );
    }

    this->on_close();
}

void ClusterNode::evt_session_receive_data( Session * , Buffer & buffer )
{
    circle_buffer_.push( buffer );
    this->parse_message();
}

void ClusterNode::evt_session_sent_complete( Session * , size_t size )
{

}

size_t ClusterNode::id()
{ 
    return this->session_ == nullptr ? 
        0 : 
        this->session_->id();
};

void ClusterNode::on_message( UPTR<Message> msg )
{

}

void ClusterNode::on_close()
{
}

void ClusterNode::parse_message()
{
    do
    {
        switch ( this->parse_state_ )
        {
            case ParseState::kFlag:
                {
                    auto t_buf = circle_buffer_.pop( 2 );

                    if ( t_buf == nullptr )
                    {
                        return;
                    }

                    if ( t_buf->data()[0] == 'Y' && t_buf->data()[1] == 'H' )
                    {
                        this->parse_state_ = ParseState::kHead;
                        break;
                    }
                }
                break;
            case ParseState::kHead:
                {
                    auto t_buf = this->circle_buffer_.pop( sizeof( short ) * 2 );

                    if ( t_buf == nullptr )
                    { 
                        return ;
                    } 

                    char* pdata = t_buf->data();
                    this->compressed_length_ = *( unsigned short* ) pdata;
                    pdata  += sizeof( short );
                    this->oringal_length_ = *( unsigned short* ) pdata;

                    if ( ( this->oringal_length_    > CIRCLE_BUFFER_SIZE ) || 
                         ( this->compressed_length_ > CIRCLE_BUFFER_SIZE ) )
                    {
                        this->parse_state_ = ParseState::kFlag;
                        break;
                    }

                    this->parse_state_ = ParseState::kBody;
                }
                break;
            case ParseState::kBody:
                {
                    auto t_buf = this->circle_buffer_.pop( this->oringal_length_ );

                    if ( t_buf == nullptr )
                    {
                        return;
                    }

                    try
                    {
                        UPTR<Message> message = MAKE_UPTR(
                            Message ,
                            std::string( t_buf->data( ) ,
                            t_buf->size( ) ) );

                        message->owner( this );
                        this->on_message( MOVE( message ) );
                        this->parse_state_ = ParseState::kFlag;
                    }
                    catch (...)
                    {
                        this->parse_state_ = ParseState::kFlag;
                        this->close( );
                        return;
                    }
                }
                break;
            default:
                break;
        }
    }
    while ( true );
}
