#ifndef QWLANMAN_DATAS_INTERFACE_H
#define QWLANMAN_DATAS_INTERFACE_H

#include "qwlanmanager/qwlanman_global.h"

#include "qwlanmanager/cache/cachepolicy.h"
#include "qwlanmanager/datas/network.h"
#include "qwlanmanager/qwlantypes.h"

#include <QUuid>

//TODO: add destructor definitions for all public classes

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class InterfaceData;
class QWLANMAN_EXPORT Interface
{
    Q_GADGET

    Q_PROPERTY(QUuid uid READ getUid)
    Q_PROPERTY(QString hwAddress READ getHwAddress)
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString description READ getDescription)
    Q_PROPERTY(CachePolicy cachePolicy READ getCachePolicy)

    friend class InterfaceMutator;

public:
    Interface();

    Interface(const Interface &other);
    Interface& operator=(const Interface &other);

public:
    bool isValid() const;

    IfaceState getState() const;
    const QUuid& getUid() const;
    const QString& getHwAddress() const;
    const QString& getName() const;
    const QString& getDescription() const;
    const CachePolicy& getCachePolicy() const;

    const MapNetworks& getMapNetworks() const;

    ListNetworks getListNetworks() const;
    Network getNetwork(const QString &ssid) const;
    Network getNetworkConnected() const;

public:
    bool operator==(const Interface &other) const;
    bool operator!=(const Interface &other) const;

private:
    QSharedPointer<InterfaceData> d_ptr;
};

/*****************************/
/* Qt specific methods       */
/*****************************/

QWLANMAN_EXPORT QDebug operator<<(QDebug debug, const Interface &interface);

/*****************************/
/* Alias for related types   */
/*****************************/

using ListInterfaces = QList<Interface>;
using MapInterfaces = QHash<QUuid, Interface>; /**< Key is interface UID */

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_INTERFACE_H
