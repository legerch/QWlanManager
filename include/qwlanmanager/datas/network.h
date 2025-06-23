#ifndef QWLANMAN_DATAS_NETWORK_H
#define QWLANMAN_DATAS_NETWORK_H

#include "qwlanmanager/qwlanman_global.h"

#include <QHash>
#include <QObject>
#include <QSharedPointer>
#include <QString>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class NetworkData;
class QWLANMAN_EXPORT Network
{
    Q_GADGET

    Q_PROPERTY(QString ssid READ getSsid)
    Q_PROPERTY(QString profile READ getProfileName)

    friend class NetworkMutator;

public:
    Network();

    Network(const Network&) = default;
    Network& operator=(const Network&) = default;

public:
    bool isValid() const;

    const QString& getSsid() const;
    const QString& getProfileName() const;

public:
    bool operator==(const Network &other) const;
    bool operator!=(const Network &other) const;

private:
    QSharedPointer<NetworkData> d_ptr;
};

/*****************************/
/* Alias for related types   */
/*****************************/

using ListNetworks = QList<Network>;
using MapNetworks = QHash<QString, Network>; /**< Key is network SSID */

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_NETWORK_H
