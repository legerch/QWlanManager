#ifndef QWLANMAN_CACHE_CACHEPOLICY_PRIV_H
#define QWLANMAN_CACHE_CACHEPOLICY_PRIV_H

#include <QtTypes>

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
    int m_maxScans;
    qint64 m_maxDelay;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_CACHE_CACHEPOLICY_PRIV_H
