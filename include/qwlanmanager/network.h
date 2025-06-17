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

private:
    QString m_bssid;
    QString m_ssid;
    QString m_profileName;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_NETWORK_H
