#include "qwlanmanager/qwlantypes.h"

#include <QHash>

/*****************************/
/* Class documentations      */
/*****************************/

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm
{

QString wlanErrorToString(WlanError idErr)
{
    static const QHash<WlanError, QString> MAP_STR_ERR =
    {
        {WlanError::WERR_NO_ERROR, "no error"},
        {WlanError::WERR_OPERATION_UNSUPPORTED, "operation unsupported"},
        {WlanError::WERR_OPERATION_CANCEL, "operation cancelled"},
        {WlanError::WERR_OPERATION_TIMEOUT, "operation reached timeout"},
        {WlanError::WERR_ITEM_INVALID, "invalid item"},
        {WlanError::WERR_IFACE_BUSY, "interface busy"},
        {WlanError::WERR_NET_UNAVAILABLE, "network unavailable"},
        {WlanError::WERR_NET_PASSKEY, "network wrong passkey"},
        {WlanError::WERR_API_INTERNAL, "engine api error"}
    };

    return MAP_STR_ERR.value(idErr, "unknown error");
}

QString wlanPermToString(WlanPerm idPerm)
{
    static const QHash<WlanPerm, QString> MAP_STR_PERMISSIONS =
    {
        {WlanPerm::WPERM_PROMPT_REQUIRED, "prompt required"},
        {WlanPerm::WPERM_GRANTED, "granted"},
        {WlanPerm::WPERM_DENIED, "denied"},
    };

    return MAP_STR_PERMISSIONS.value(idPerm, "unknown");
}

QString requestTypeToString(RequestType idReq)
{
    static const QHash<RequestType, QString> MAP_STR_REQS =
    {
        {RequestType::REQ_SCAN, "scan"},
        {RequestType::REQ_CONNECT, "connect"},
        {RequestType::REQ_DISCONNECT, "disconnect"},
    };

    return MAP_STR_REQS.value(idReq, "unknown");
}

QString ifaceStateToString(IfaceState idState)
{
    static const QHash<IfaceState, QString> MAP_STR_IFACE_STS =
    {
        {IfaceState::IFACE_STS_IDLE, "idle"},
        {IfaceState::IFACE_STS_SCANNING, "scanning"},
        {IfaceState::IFACE_STS_CONNECTING, "connecting"}
    };

    return MAP_STR_IFACE_STS.value(idState, "unknown state");
}

QString authAlgoToString(AuthAlgo idAuth)
{
    static const QHash<AuthAlgo, QString> MAP_STR_AUTH =
    {
        {AuthAlgo::AUTH_ALGO_OPEN, "Open authentication"},
        {AuthAlgo::AUTH_ALGO_WPA2_PERSONAL, "WPA2 Personal"},
        {AuthAlgo::AUTH_ALGO_WPA2_ENTERPRISE, "WPA2 Enterprise"},
        {AuthAlgo::AUTH_ALGO_WPA3_PERSONAL, "WPA3 Personal"},
        {AuthAlgo::AUTH_ALGO_WPA3_ENTERPRISE, "WPA3 Enterprise"},
    };

    return MAP_STR_AUTH.value(idAuth, "Unknown authentication algorithm");
}

QString cipherAlgoToString(CipherAlgo idCipher)
{
    static const QHash<CipherAlgo, QString> MAP_STR_CIPHER =
    {
        {CipherAlgo::CIPHER_ALGO_NONE, "None"},
        {CipherAlgo::CIPHER_ALGO_TKIP, "TKIP"},
        {CipherAlgo::CIPHER_ALGO_AES, "AES"},
        {CipherAlgo::CIPHER_ALGO_GCMP, "GCMP"},
    };

    return MAP_STR_CIPHER.value(idCipher, "Unknown cipher algorithm");
}

/*****************************/
/* Functions implementation  */
/*****************************/

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
