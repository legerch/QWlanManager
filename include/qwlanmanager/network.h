#ifndef QWLANMAN_NETWORK_H
#define QWLANMAN_NETWORK_H

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

class Network
{
    Q_GADGET

    Q_PROPERTY(QString bssid READ getBssid)
    Q_PROPERTY(QString ssid READ getSsid)
    Q_PROPERTY(QString profile READ getProfileName)    

public:
    Network();

public:
    const QString& getBssid() const;
    const QString& getSsid() const;
    const QString& getProfileName() const;

public:
    bool operator==(const Network &other) const;
    bool operator!=(const Network &other) const;
    bool operator<(const Network &other) const;
    bool operator>(const Network &other) const;
    bool operator<=(const Network &other) const;
    bool operator>=(const Network &other) const;

private:
    QString m_bssid;
    QString m_ssid;
    QString m_profileName;
};

/*****************************/
/* Qt custom related methods */
/*****************************/
size_t qHash(const Network &key, uint seed = 0);

/*****************************/
/* Alias for related types   */
/*****************************/
using ListNetworks = QList<Network>;
using MapNetworks = QHash<QString, Network>; /**< Key is network BSSID */

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_NETWORK_H
