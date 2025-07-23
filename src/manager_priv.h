#ifndef QWLANMAN_MANAGER_PRIV_H
#define QWLANMAN_MANAGER_PRIV_H

#include "datas/interfacemutator.h"
#include "datas/networkmutator.h"

#include "qwlanmanager/manager.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class ManagerPrivate
{
    QWLANMAN_DISABLE_COPY(ManagerPrivate)
    Q_DECLARE_PUBLIC(Manager)

public:
    explicit ManagerPrivate(Manager *parent);

    virtual ~ManagerPrivate();

public:
    void interfaceListUpdate();

    void interfaceScanNetworks(Interface interface);
    void interfaceConnect(Interface interface, Network network, const QString &password);
    void interfaceDisconnect(Interface interface);
    void interfaceForget(Interface interface, Network network);

public:
    virtual void initialize() = 0;
    virtual void terminate() = 0;

protected:
    virtual void interfaceListRefresh() = 0;

    virtual void interfaceScanNetworksAsync(Interface interface) = 0;
    virtual void interfaceConnectAsync(Interface interface, Network network, const QString &password) = 0;
    virtual void interfaceDisconnectAsync(Interface interface) = 0;
    virtual void interfaceForgetAsync(Interface interface, Network network) = 0;

protected:
    void handleInterfacesListUpdateDone();

    void handleScanDone(const Interface &interface, WlanError idErr);
    void handleConnectDone(const Interface &interface, const QString &ssid, WlanError idErr);
    void handleDisconnectDone(const Interface &interface, WlanError idErr);
    void handleForgetDone(const Interface &interface, const Network &network, WlanError idErr);

private:
    void requestAdd(const Interface &interface, const Request &req);
    void requestPerform(const Interface &interface, const Request &req);
    void requestHandleDone(const Interface &interface, std::function<void()> cbSignals);

protected:
    MapInterfaces m_currentIfaces;
    MapInterfaces m_prevIfaces;
    WlanOptions m_opts;

    Manager *q_ptr;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_MANAGER_PRIV_H
