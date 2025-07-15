#ifndef QWLANMAN_CACHE_CACHEPOLICY_PRIV_H
#define QWLANMAN_CACHE_CACHEPOLICY_PRIV_H

#include <QtGlobal> // The day we drop Qt5, this include could be replaced by <QtTypes>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class CachePolicyPrivate
{

public:
    CachePolicyPrivate();

public:
    bool operator==(const CachePolicyPrivate &other) const;
    bool operator!=(const CachePolicyPrivate &other) const;

public:
    int m_maxScans = 0;
    qint64 m_maxDelay = 0;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_CACHE_CACHEPOLICY_PRIV_H
