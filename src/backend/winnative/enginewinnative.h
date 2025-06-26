#ifndef QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H
#define QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H

#include "manager_priv.h"
#include "typeswinnative.h"

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

private:
    bool apiOpen();
    void apiClose();

    bool eventRegister();
    void eventUnregister();

    bool interfaceIsVirtual(const QString &description);

    void interfaceListHandleEvents(const MapInterfaces &oldMap, const MapInterfaces &newMap);
    WlanError interfaceNetworksUpdate(Interface interface);
    WlanError interfaceNetworkCreateProfile(Interface interface, Network network, const QString &password);

    void interfaceScanFinished(const QUuid &idInterface, WlanError result);
    void interfaceConnectionFinished(const QUuid &idInterface, const QString &ssid, WlanError result);

private:
    static void WINAPI cbNotif(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx);

    static void WINAPI cbNotifAcm(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx);
    static void WINAPI cbNotifMsm(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx);

private:
    Qt::HANDLE m_handle;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H
