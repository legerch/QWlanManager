#include "cacheinfo.h"

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
/*****************************/

CacheInfo::CacheInfo()
{
    /* Nothing to do */
}

void CacheInfo::markSeen(const QDateTime &now)
{
    m_nbScansMiss = 0;
    m_lastSeen = now;
}

void CacheInfo::markUnseen()
{
    ++m_nbScansMiss;
}

bool CacheInfo::isExpired(const CachePolicy &policy, const QDateTime &now) const
{
    /* Verify if we reach maximum number of missed scans */
    if(m_nbScansMiss > policy.getMaxScans()){
        return true;
    }

    /* Verify if we reach maximum delays since we seen it */
    if(m_lastSeen.secsTo(now) > policy.getMaxDelay()){
        return true;
    }

    return false;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
