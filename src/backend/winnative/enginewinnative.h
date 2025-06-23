#ifndef QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H
#define QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H

#include "manager_priv.h"

/*
 * Since we need to include winsock2.h, we need to define WIN32_LEAN_AND_MEAN
 * so windows.h won't include winsock.h.
 *
 * Note: This definition is commented in this file because
 * it is defined in top layer (see build system, which is currently cmake)
 */
//#define WIN32_LEAN_AND_MEAN 1

#include <Windows.h>
#include <Wlanapi.h>

/*
 * IpHlpAPI : Windows API which contains tools to manage interfaces
 * and IPs addresses.
 * In addition, we need to include winsock2.h before iphlpapi.h and we need
 * to include ws2ipdef.h to work around a MinGW-w64 bug
 * (http://sourceforge.net/p/mingw-w64/mailman/message/32935366/)
 */
#include <winsock2.h>
#include <ws2ipdef.h>
#include <wincrypt.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

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

private:
    bool apiOpen();
    void apiClose();

    bool eventRegister();
    void eventUnregister();

    bool interfaceIsVirtual(const QString &description);

    void interfaceListHandleEvents(const MapInterfaces &oldMap, const MapInterfaces &newMap);
    WlanError interfaceNetworksUpdate(Interface interface);
    void interfaceScanFinished(const QUuid &idInterface, WlanError result);

private:
    static WlanError convertWinNativeErr(WLAN_REASON_CODE winErr);

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
