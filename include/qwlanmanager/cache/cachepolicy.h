#ifndef QWLANMAN_CACHE_CACHEPOLICY_H
#define QWLANMAN_CACHE_CACHEPOLICY_H

#include "qwlanmanager/qwlanman_global.h"

#include <QDebug>
#include <QObject>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class CachePolicyPrivate;
class QWLANMAN_EXPORT CachePolicy
{
    Q_GADGET

    Q_PROPERTY(int maxScans READ getMaxScans WRITE setMaxScans)
    Q_PROPERTY(int maxDelay READ getMaxDelay WRITE setMaxDelay)

public:
    CachePolicy();
    CachePolicy(int maxScans, qint64 maxDelay);

    CachePolicy(const CachePolicy &other);
    CachePolicy(CachePolicy &&other);

    ~CachePolicy();

public:
    int getMaxScans() const;
    qint64 getMaxDelay() const;

public:
    void setMaxScans(int maxScans);
    void setMaxDelay(qint64 maxDelay);

public:
    bool operator==(const CachePolicy &other) const;
    bool operator!=(const CachePolicy &other) const;

public:
    CachePolicy& operator=(const CachePolicy &other);
    CachePolicy& operator=(CachePolicy &&other) noexcept;

private:
    std::unique_ptr<CachePolicyPrivate> d_ptr;
};

/*****************************/
/* Qt specific methods       */
/*****************************/

QWLANMAN_EXPORT QDebug operator<<(QDebug debug, const CachePolicy &policy);

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

/*****************************/
/* Qt specific meta-system   */
/*****************************/

Q_DECLARE_METATYPE(qwm::CachePolicy)

#endif // QWLANMAN_CACHE_CACHEPOLICY_H
