#ifndef QWLANMAN_DATAS_INTERFACE_H
#define QWLANMAN_DATAS_INTERFACE_H

#include "qwlanmanager/qwlanman_global.h"

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

    friend class InterfaceMutator;

public:
    Interface();

    Interface(const Interface&) = default;
    Interface& operator=(const Interface&) = default;

public:
    bool isValid() const;

    IfaceState getState() const;
    const QUuid& getUid() const;
    const QString& getHwAddress() const;
    const QString& getName() const;
    const QString& getDescription() const;

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
/* Alias for related types   */
/*****************************/

using ListInterfaces = QList<Interface>;
using MapInterfaces = QHash<QUuid, Interface>; /**< Key is interface UID */

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_INTERFACE_H
