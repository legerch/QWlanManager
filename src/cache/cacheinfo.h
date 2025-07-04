#ifndef QWLANMAN_CACHE_CACHEINFO_H
#define QWLANMAN_CACHE_CACHEINFO_H

#include "qwlanmanager/cache/cachepolicy.h"

#include <QDateTime>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class CacheInfo
{

public:
    CacheInfo();

public:
    void markSeen(const QDateTime &now);
    void markUnseen();

public:
    bool isExpired(const CachePolicy &policy, const QDateTime &now) const;

private:
    int m_nbScansMiss = 0;
    QDateTime m_lastSeen;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_CACHE_CACHEINFO_H
