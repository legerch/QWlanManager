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

    enum class WlanPerm
    {
        WPERM_UNKNOWN = 0,

        WPERM_PROMPT_REQUIRED,
        WPERM_GRANTED,
        WPERM_DENIED
    };
    Q_ENUM_NS(WlanPerm)

    QWLANMAN_EXPORT QString wlanPermToString(WlanPerm idPerm);

    enum class RequestType
    {
        REQ_INVALID = 0,

        REQ_SCAN,
        REQ_CONNECT,
        REQ_DISCONNECT
    };
    Q_ENUM_NS(RequestType);

    QWLANMAN_EXPORT QString requestTypeToString(RequestType idReq);

    enum class IfaceState
    {
        IFACE_STS_IDLE = 0,
        IFACE_STS_SCANNING,
        IFACE_STS_CONNECTING,
        IFACE_STS_DISCONNECTING
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
/* Qt related methods        */
/*****************************/

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

inline uint qHash(qwm::WlanError key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::WlanError>::type>(key), seed);
}

inline uint qHash(qwm::WlanPerm key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::WlanPerm>::type>(key), seed);
}

inline uint qHash(qwm::RequestType key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::RequestType>::type>(key), seed);
}

inline uint qHash(qwm::IfaceState key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::IfaceState>::type>(key), seed);
}

inline uint qHash(qwm::AuthAlgo key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::AuthAlgo>::type>(key), seed);
}

inline uint qHash(qwm::CipherAlgo key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::CipherAlgo>::type>(key), seed);
}

#endif

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_QWLANTYPES_H
