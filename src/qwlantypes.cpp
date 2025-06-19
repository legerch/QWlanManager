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

QString ifaceStateToString(IfaceState idState)
{
    static const QHash<IfaceState, QString> MAP_STR_ERR =
    {
        {IfaceState::IFACE_STS_IDLE, "idle"},
        {IfaceState::IFACE_STS_SCANNING, "scanning"}
    };

    return MAP_STR_ERR.value(idState, "unknown state");
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
