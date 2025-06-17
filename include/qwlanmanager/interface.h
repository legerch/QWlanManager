#ifndef QWLANMAN_INTERFACE_H
#define QWLANMAN_INTERFACE_H

#include <QHash>
#include <QObject>
#include <QUuid>

#include "qwlanmanager/network.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

//TODO: when retrieveing interfaces, filter "virtual"

class Interface
{
    Q_GADGET

    Q_PROPERTY(QUuid uid READ getUid)
    Q_PROPERTY(QString hwAddress READ getHwAddress)
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString friendlyName READ getFriendlyName)
    Q_PROPERTY(QString description READ getDescription)
    Q_PROPERTY(bool isUp READ isUp)

public:
    using MapNetworks = QHash<QString, Network>; /**< Key is network BSSID */

public:
    Interface();

public:
    const QUuid& getUid() const;
    const QString& getHwAddress() const;
    const QString& getName() const;
    const QString& getFriendlyName() const;
    const QString& getDescription() const;
    bool isUp() const;

    const MapNetworks& getMapNetworks() const;

private:
    QUuid m_uid;
    QString m_hwAddress;
    QString m_name;
    QString m_friendlyName;
    QString m_description;
    bool m_isUp;

    MapNetworks m_mapNets;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_INTERFACE_H
