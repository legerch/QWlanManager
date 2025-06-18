#ifndef QWLANMAN_NETWORK_H
#define QWLANMAN_NETWORK_H

#include "qwlanmanager/qwlanman_global.h"

#include <QHash>
#include <QSharedPointer>
#include <QString>
#include <QObject>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class QWLANMAN_EXPORT NetworkData
{
    Q_GADGET

    Q_PROPERTY(QString bssid READ getBssid)
    Q_PROPERTY(QString ssid READ getSsid)
    Q_PROPERTY(QString profile READ getProfileName)    

public:
    NetworkData();

public:
    const QString& getBssid() const;
    const QString& getSsid() const;
    const QString& getProfileName() const;

public:
    bool operator==(const NetworkData &other) const;
    bool operator!=(const NetworkData &other) const;
    bool operator<(const NetworkData &other) const;
    bool operator>(const NetworkData &other) const;
    bool operator<=(const NetworkData &other) const;
    bool operator>=(const NetworkData &other) const;

private:
    QString m_bssid;
    QString m_ssid;
    QString m_profileName;
};

/*****************************/
/* Qt custom related methods */
/*****************************/
size_t qHash(const NetworkData &key, uint seed = 0);

/*****************************/
/* Alias for related types   */
/*****************************/
using Network = QSharedPointer<NetworkData>;

using ListNetworks = QList<Network>;
using MapNetworks = QHash<QString, Network>; /**< Key is network BSSID */

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_NETWORK_H
