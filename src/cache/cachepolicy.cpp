#include "qwlanmanager/cache/cachepolicy.h"

#include "cachepolicy_priv.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class qwm::CachePolicy
 *
 * \brief Class allowing to determine the cache policy to adopt
 * \details
 * Cache policy feature allow to workaround some "weird" behaviour from the native API. \n
 * For example, we can perform a scan and retrieve 20 networks, later performing the same scan
 * (all networks are still available) but due to some internal behaviour, scan can returns [0-4] networks
 * over the 20 available. \n
 * The cache policy allow to set a network cache, allowing to remove a network from our available list
 * only if cache has expired. \n
 * This class is also compatible with \b QML.
 *
 * \sa Manager::setCachePolicy()
 */

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

/*!
 * \brief Set a default cache policy
 * \details
 * This is the default cache policy, keeping the native behaviour.
 */
CachePolicy::CachePolicy()
    : CachePolicy(0, 0)
{
    /* Nothing to do, use constructor delegation */
}

/*!
 * \brief Set a custom cache policy
 *
 * \param[in] maxScans
 * Maximum number of scan to miss to consider a network \em unavailable.
 * \param[in] maxDelay
 * Maximum delay in milliseconds to consider a network \em unavailable.
 *
 * \sa setMaxScans(), setMaxDelay()
 */
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

/*!
 * \brief Retrieve maximum number of scans
 * to miss to consider a network as \em unavailable.
 *
 * \return
 * Returns maximum number of scans.
 *
 * \sa setMaxScans()
 * \sa getMaxDelay()
 */
int CachePolicy::getMaxScans() const
{
    return d_ptr->m_maxScans;
}

/*!
 * \brief Retrieve maximum delay in milliseconds
 * to consider a network as \em unavailable.
 *
 * \return
 * Returns maximum delay in milliseconds.
 *
 * \sa setMaxDelay()
 * \sa getMaxScans()
 */
qint64 CachePolicy::getMaxDelay() const
{
    return d_ptr->m_maxDelay;
}

/*!
 * \brief Set maximum scans to miss to consider
 * a network as \em unavailable.
 *
 * \param[in] maxScans
 * Maximum number of scan to miss to consider a network \em unavailable.
 *
 * \sa setMaxDelay()
 * \sa getMaxScans()
 */
void CachePolicy::setMaxScans(int maxScans)
{
    d_ptr->m_maxScans = maxScans;
}

/*!
 * \brief Set maximum delay to consider
 * a network as \em unavailable.
 *
 * \param[in] maxDelay
 * Maximum delay in milliseconds to consider a network \em unavailable.
 *
 * \sa getMaxDelay()
 * \sa setMaxScans()
 */
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
