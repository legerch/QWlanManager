#ifndef QWLANMAN_DATAS_INTERFACE_H
#define QWLANMAN_DATAS_INTERFACE_H

#include "qwlanmanager/qwlanman_global.h"

#include "qwlanmanager/cache/cachepolicy.h"
#include "qwlanmanager/datas/network.h"
#include "qwlanmanager/qwlantypes.h"

#include <QUuid>

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

    Q_PROPERTY(CachePolicy cachePolicy READ getCachePolicy WRITE setCachePolicy)
    Q_PROPERTY(IfaceOptions options READ getOptions WRITE setOptions)

    friend class InterfaceMutator;

public:
    Interface();

    Interface(const Interface &other);
    Interface(Interface &&other);

    ~Interface();

public:
    bool isValid() const;
    bool isBusy() const;

    IfaceState getState() const;
    IfaceOptions getOptions() const;

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
    void setCachePolicy(const CachePolicy &cachePolicy);
    void setOptions(IfaceOptions opts);

public:
    Interface& operator=(const Interface &other);
    Interface& operator=(Interface &&other) noexcept;

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

/*****************************/
/* Qt specific meta-system   */
/*****************************/

Q_DECLARE_METATYPE(qwm::Interface)

#endif // QWLANMAN_DATAS_INTERFACE_H
