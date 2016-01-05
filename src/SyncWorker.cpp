#include "SyncWorker.h"

NS_MARATON_BEGIN

SyncWorker* SyncWorker::create( const syncworker_callback_t  work_callback , 
                         const syncworker_callback_t  after_callback , 
                         void * data )
{
    SyncWorker* worker      = new SyncWorker();
    worker->cb_work_        = work_callback;
    worker->cb_after_work_  = after_callback;
    worker->loop_time_      = 1;
    worker->data( data );
    worker->start();

    return worker;
}

SyncWorker* SyncWorker::create( const size_t time_span , 
                         const syncworker_callback_t work_callback , 
                         const syncworker_callback_t after_callback , 
                         void * data )
{
    SyncWorker* worker      = new SyncWorker();
    worker->cb_work_        = work_callback;
    worker->cb_after_work_  = after_callback;
    worker->loop_time_      = time_span;
    worker->data( data );
    worker->start();

    return worker;
}

void SyncWorker::stop( SyncWorker * worker )
{
    if ( worker == nullptr )
    {
        return;
    }

    worker->stop( );
}

void SyncWorker::stop()
{
    this->finished_ = true;
}

void SyncWorker::uv_process_timer_tick_callback( uv_timer_t * handle )
{
    SyncWorker* worker = static_cast< SyncWorker* >( handle->data );

    if ( worker->finished_ )
    { 
        if ( worker->cb_after_work_ != nullptr )
        {
            worker->cb_after_work_( worker );
        }

        int result = uv_timer_stop( &worker->timer_ );

        LOG_DEBUG_UV( result );
        
        SAFE_DELETE( worker );

        return;
    }

    if( worker->loop_count_ == 0)
    {
        worker->loop_count_++;
        return;
    }

    if ( worker == nullptr )
    {
        uv_timer_stop( handle );
        return;
    }

    if ( !worker->finished_ )
    {
        worker->finished_  = worker->cb_work_( worker );
        ++worker->loop_count_;
    }

    /*if ( worker->finished_ )
    { 
        if ( worker->cb_after_work_ != nullptr )
        {
            worker->cb_after_work_( worker );
        }

        int result = uv_timer_stop( &worker->timer_ );

        LOG_DEBUG_UV( result );
        
        SAFE_DELETE( worker );
    }*/
}

SyncWorker::SyncWorker()
{
    this->timer_        = { 0 };
    this->timer_.data   = this;
    this->loop_count_   = 0;
    this->loop_time_    = 1;

    uv_timer_init( uv_default_loop() , &this->timer_ );
}

SyncWorker::~SyncWorker()
{
}

void SyncWorker::start()
{
    uv_timer_start( &this->timer_ , 
                    SyncWorker::uv_process_timer_tick_callback , 
                    0 , 
                    this->loop_time_ );
}

NS_MARATON_END