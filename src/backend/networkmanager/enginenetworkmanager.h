#ifndef QWLANMAN_BACKEND_NM_ENGINENETWORKMANAGER_H
#define QWLANMAN_BACKEND_NM_ENGINENETWORKMANAGER_H

#include "manager_priv.h"

#include "deviceinfoprovider.h"

#include <QDBusMessage>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/* EngineNetworkManager      */
/*****************************/

class EngineNetworkManager;
class DelegateNetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit DelegateNetworkManager(EngineNetworkManager *engine);
    ~DelegateNetworkManager();

public:
    void eventRegister();
    void eventUnregister();

private slots:
    void handleDeviceAdded(const QDBusMessage &msg);
    void handleDeviceRemoved(const QDBusMessage &msg);

private:
    EngineNetworkManager *m_engine = nullptr;
};

/*****************************/
/* Class definitions         */
/* EngineNetworkManager      */
/*****************************/

class EngineNetworkManager final : public ManagerPrivate
{
public:
    explicit EngineNetworkManager(Manager *parent);
    ~EngineNetworkManager();

public:
    void initialize() override;
    void terminate() override;

protected:
    void interfaceListRefresh() override;
    void interfaceScanNetworksAsync(Interface interface) override;
    void interfaceConnectAsync(Interface interface, Network network, const QString &password) override;
    void interfaceDisconnectAsync(Interface interface) override;
    void interfaceForgetAsync(Interface interface, Network network) override;

private:
    DeviceInfosProvider m_devProvider;
    DelegateNetworkManager m_delegate;

private:
    static const QUuid NM_UID;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_NM_ENGINENETWORKMANAGER_H
