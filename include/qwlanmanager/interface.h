#ifndef QWLANMAN_INTERFACE_H
#define QWLANMAN_INTERFACE_H

#include "qwlanmanager/qwlanman_global.h"
#include "qwlanmanager/network.h"

#include <QUuid>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

//TODO: when retrieveing interfaces, filter "virtual"

class QWLANMAN_EXPORT InterfaceData
{
    Q_GADGET

    Q_PROPERTY(QUuid uid READ getUid)
    Q_PROPERTY(QString hwAddress READ getHwAddress)
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString description READ getDescription)

    friend class IfaceMutator;

public:
    InterfaceData();

public:
    const QUuid& getUid() const;
    const QString& getHwAddress() const;
    const QString& getName() const;
    const QString& getDescription() const;

    const MapNetworks& getMapNetworks() const;

    ListNetworks getListNetworks() const;
    Network getNetwork(const QString &ssid) const;

public:
    bool operator==(const InterfaceData &other) const;
    bool operator!=(const InterfaceData &other) const;
    bool operator<(const InterfaceData &other) const;
    bool operator>(const InterfaceData &other) const;
    bool operator<=(const InterfaceData &other) const;
    bool operator>=(const InterfaceData &other) const;

private:
    QUuid m_uid;
    QString m_hwAddress;
    QString m_name;
    QString m_description;

    MapNetworks m_mapNets;
};

/*****************************/
/* Qt custom related methods */
/*****************************/
size_t qHash(const InterfaceData &key, uint seed = 0);

/*****************************/
/* Alias for related types   */
/*****************************/

using Interface = QSharedPointer<InterfaceData>;

using ListInterfaces = QList<Interface>;
using MapInterfaces = QHash<QUuid, Interface>; /**< Key is interface UID */

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_INTERFACE_H
