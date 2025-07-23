#ifndef QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H
#define QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H

#include "manager_priv.h"
#include "typeswinnative.h" // IWYU pragma: keep

#include <QSet>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class EngineWinNative final : public ManagerPrivate
{
public:
    explicit EngineWinNative(Manager *parent);
    ~EngineWinNative();

public:
    void initialize() override;
    void terminate() override;

public:
    void interfaceListRefresh() override;

protected:
    void interfaceScanNetworksAsync(Interface interface) override;
    void interfaceConnectAsync(Interface interface, Network network, const QString &password) override;
    void interfaceDisconnectAsync(Interface interface) override;
    void interfaceForgetAsync(Interface interface, Network network) override;

private:
    bool apiOpen();
    void apiClose();

    bool eventRegister();
    void eventUnregister();

    bool interfaceIsVirtual(const QString &description);

    WlanError interfaceNetworksUpdate(Interface interface);
    WlanError interfaceNetworkProfileCreate(Interface interface, Network network, const QString &password);
    WlanError interfaceNetworkProfileDelete(Interface interface, Network network);

    void interfaceScanFinished(const QUuid &idInterface, WlanError result);
    void interfaceConnectionFinished(const QUuid &idInterface, const QString &ssid, WlanError result);
    void interfaceDisconnectionFinished(const QUuid &idInterface, WlanError result);

    void interfaceSignalQualityReceived(const QUuid &idInterface, uint percent);

private:
    static void WINAPI cbNotif(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx);

    static void WINAPI cbNotifAcm(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx);
    static void WINAPI cbNotifMsm(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx);

private:
    Qt::HANDLE m_handle;
    QHash<QUuid, WLAN_REASON_CODE> m_errConnect;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H
