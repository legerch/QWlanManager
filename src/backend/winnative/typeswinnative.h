#ifndef QWLANMAN_BACKEND_WINNATIVE_TYPESWINNATIVE_H
#define QWLANMAN_BACKEND_WINNATIVE_TYPESWINNATIVE_H

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

#include <QObject>

#include "qwlanmanager/qwlantypes.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm::WinNative
{
    Q_NAMESPACE

    WlanError convertErrWlanFromApi(WLAN_REASON_CODE apiErr);
    WlanError convertErrWinFromApi(DWORD apiErr);

    AuthAlgo convertAuthFromApi(DOT11_AUTH_ALGORITHM apiAuth);
    CipherAlgo convertCipherFromApi(DOT11_CIPHER_ALGORITHM apiCipher);

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm::WinNative

#endif // QWLANMAN_BACKEND_WINNATIVE_TYPESWINNATIVE_H
