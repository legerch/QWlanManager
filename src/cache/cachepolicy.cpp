#include "qwlanmanager/cache/cachepolicy.h"

#include "cachepolicy_priv.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*****************************/
/* Signals documentations    */
/*****************************/


/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm
{

/*****************************/
/* Functions implementation  */
/*      Public Class         */
/*****************************/

CachePolicy::CachePolicy()
    : CachePolicy(0, 0)
{
    /* Nothing to do, use constructor delegation */
}

CachePolicy::CachePolicy(int maxScans, qint64 maxDelay)
    : d_ptr(std::make_unique<CachePolicyPrivate>())
{
    setMaxScans(maxScans);
    setMaxDelay(maxDelay);
}

CachePolicy::CachePolicy(const CachePolicy &other)
    : d_ptr(std::make_unique<CachePolicyPrivate>(*other.d_ptr)){}

CachePolicy::CachePolicy(CachePolicy &&other)
    : d_ptr(std::move(other.d_ptr)){}

CachePolicy::~CachePolicy() = default;

int CachePolicy::getMaxScans() const
{
    return d_ptr->m_maxScans;
}

qint64 CachePolicy::getMaxDelay() const
{
    return d_ptr->m_maxDelay;
}

void CachePolicy::setMaxScans(int maxScans)
{
    d_ptr->m_maxScans = maxScans;
}

void CachePolicy::setMaxDelay(qint64 maxDelay)
{
    d_ptr->m_maxDelay = maxDelay;
}

bool CachePolicy::operator==(const CachePolicy &other) const
{
    /* Do one is null ? Do the two are null ? */
    if(!d_ptr || !other.d_ptr){
        return (!d_ptr && !other.d_ptr);
    }

    /* Compare content */
    return *d_ptr == *other.d_ptr;
}

bool CachePolicy::operator!=(const CachePolicy &other) const
{
    return !(*this == other);
}

CachePolicy& CachePolicy::operator=(const CachePolicy &other)
{
    /* Verify that value actually differs */
    if(this == &other){
        return *this;
    }

    /* Perform copy assignment */
    d_ptr = std::make_unique<CachePolicyPrivate>(*other.d_ptr);
    return *this;
}

CachePolicy& CachePolicy::operator=(CachePolicy &&other) noexcept
{
    d_ptr = std::move(other.d_ptr);
    return *this;
}

/*****************************/
/* Qt specific methods       */
/*****************************/

QDebug operator<<(QDebug debug, const CachePolicy &policy)
{
    QDebugStateSaver saver(debug);

    debug.nospace() << "CachePolicy("
                    << "max scans: " << policy.getMaxScans() << ", "
                    << "max delay: " << policy.getMaxDelay() << ")";
    return debug;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
