#ifndef QWLANMAN_DATAS_NETWORK_H
#define QWLANMAN_DATAS_NETWORK_H

#include "qwlanmanager/qwlanman_global.h"
#include "qwlanmanager/qwlantypes.h"

#include <QDebug>
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
    Q_PROPERTY(AuthAlgo authAlgo READ getAuthAlgo)
    Q_PROPERTY(CipherAlgo cipherAlgo READ getCipherAlgo)
    Q_PROPERTY(uint signalQuality READ getSignalQuality)

    friend class NetworkMutator;

public:
    Network();

    Network(const Network &other);
    Network& operator=(const Network &other);

public:
    bool isValid() const;

    const QString& getSsid() const;
    const QString& getProfileName() const;
    AuthAlgo getAuthAlgo() const;
    CipherAlgo getCipherAlgo() const;
    uint getSignalQuality() const;

public:
    bool operator==(const Network &other) const;
    bool operator!=(const Network &other) const;

private:
    QSharedPointer<NetworkData> d_ptr;
};

/*****************************/
/* Qt specific methods       */
/*****************************/

QWLANMAN_EXPORT QDebug operator<<(QDebug debug, const Network &net);

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
