#include "Feature.h"

Feature::Feature()
{
    service_ = MAKE_UPTR( NetworkService );
    service_->register_notifier( this );
}

Feature::~Feature()
{

}
