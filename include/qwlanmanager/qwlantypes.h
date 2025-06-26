#ifndef QWLANMAN_QWLANTYPES_H
#define QWLANMAN_QWLANTYPES_H

#include "qwlanman_global.h"

#include <QObject>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{
    Q_NAMESPACE_EXPORT(QWLANMAN_EXPORT)

    enum class WlanError
    {
        WERR_NO_ERROR = 0,
        WERR_UNKNOWN,

        WERR_OPERATION_UNSUPPORTED,
        WERR_OPERATION_CANCEL,
        WERR_OPERATION_TIMEOUT,
        WERR_ITEM_INVALID,
        WERR_IFACE_BUSY,
        WERR_NET_UNAVAILABLE,
        WERR_NET_PASSKEY,
        WERR_API_INTERNAL
    };
    Q_ENUM_NS(WlanError)

    QWLANMAN_EXPORT QString wlanErrorToString(WlanError idErr);

    enum class IfaceState
    {
        IFACE_STS_IDLE = 0,
        IFACE_STS_SCANNING,
        IFACE_STS_CONNECTING
    };
    Q_ENUM_NS(IfaceState);

    QWLANMAN_EXPORT QString ifaceStateToString(IfaceState idState);

    enum class AuthAlgo
    {
        AUTH_ALGO_UNKNOWN = 0,

        AUTH_ALGO_OPEN,
        AUTH_ALGO_WPA2_PERSONAL,
        AUTH_ALGO_WPA2_ENTERPRISE,
        AUTH_ALGO_WPA3_PERSONAL,
        AUTH_ALGO_WPA3_ENTERPRISE
    };
    Q_ENUM_NS(AuthAlgo);

    QWLANMAN_EXPORT QString authAlgoToString(AuthAlgo idAuth);

    enum class CipherAlgo
    {
        CIPHER_ALGO_UNKNOWN = 0,

        CIPHER_ALGO_NONE,
        CIPHER_ALGO_TKIP,
        CIPHER_ALGO_AES,
        CIPHER_ALGO_GCMP
    };
    Q_ENUM_NS(CipherAlgo);

    QWLANMAN_EXPORT QString cipherAlgoToString(CipherAlgo idCipher);

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_QWLANTYPES_H
