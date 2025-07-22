#ifndef QWLANMAN_MANAGER_H
#define QWLANMAN_MANAGER_H

#include "qwlanmanager/qwlanman_global.h"

#include "qwlanmanager/datas/interface.h"
#include "qwlanmanager/qwlantypes.h"

#include <QObject>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class ManagerPrivate;
class QWLANMAN_EXPORT Manager : public QObject
{
    Q_OBJECT
    QWLANMAN_DISABLE_COPY_MOVE(Manager)

public:
    explicit Manager(QObject *parent = nullptr);
    ~Manager();

public:
    void doScan(const QUuid &idInterface);
    void doConnect(const QUuid &idInterface, const QString &ssid, const QString &password = "");
    void doDisconnect(const QUuid &idInterface);
    void doForget(const QUuid &idInterface, const QString &ssid);

public:
    ListInterfaces getInterfaces() const;
    Interface getInterface(const QUuid &idInterface) const;

signals:
    void sInterfaceAdded(qwm::Interface interface);
    void sInterfaceRemoved(qwm::Interface interface);

    void sScanStarted(const QUuid &idInterface);
    void sScanSucceed(const QUuid &idInterface, qwm::ListNetworks listNets);
    void sScanFailed(const QUuid &idInterface, qwm::WlanError idErr);

    void sConnectionStarted(const QUuid &idInterface, const QString &ssid);
    void sConnectionSucceed(const QUuid &idInterface, const QString &ssid);
    void sConnectionFailed(const QUuid &idInterface, const QString &ssid, qwm::WlanError idErr);

    void sDisconnectionStarted(const QUuid &idInterface);
    void sDisconnectionSucceed(const QUuid &idInterface);
    void sDisconnectionFailed(const QUuid &idInterface, qwm::WlanError idErr);

    void sForgetStarted(const QUuid &idInterface, const QString &ssid);
    void sForgetSucceed(const QUuid &idInterface, const QString &ssid);
    void sForgetFailed(const QUuid &idInterface, const QString &ssid, qwm::WlanError idErr);

    void sSignalQualityChanged(const QUuid &idInterface, uint percent);

private:
    std::unique_ptr<ManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Manager)
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_MANAGER_H
