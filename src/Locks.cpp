#include "..\inc\Locks.h"

NS_MARATON_BEGIN

Mutex::Mutex( )
{
    uv_mutex_init( &this->mutex_ );
}

Mutex::~Mutex( )
{
    uv_mutex_destroy( &this->mutex_ );
}

void Mutex::wait( )
{
    uv_mutex_lock( &this->mutex_ );
}

void Mutex::release( )
{
    uv_mutex_unlock( &this->mutex_ );
}
 
Semaphore::Semaphore( )
{
    uv_sem_init( &this->sem_ , 0 );
}

Semaphore::Semaphore( int initNum )
{
     uv_sem_init( &this->sem_ , initNum );
}

Semaphore::~Semaphore( )
{
    uv_sem_destroy( &this->sem_ );
}

void Semaphore::wait( )
{
    uv_sem_wait( &this->sem_ );
}

void Semaphore::release( )
{
    uv_sem_post( &this->sem_ );
}

NS_MARATON_END
