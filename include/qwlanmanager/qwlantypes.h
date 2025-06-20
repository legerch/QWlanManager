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
        WERR_OPERATION_CANCEL,
        WERR_OPERATION_TIMEOUT,
        WERR_ITEM_INVALID,
        WERR_IFACE_BUSY,
        WERR_NET_UNAVAILABLE,
        WERR_NET_PASSKEY,
        WERR_API_INTERNAL
    };
    Q_ENUM_NS(WlanError)

    QString wlanErrorToString(WlanError idErr);

    enum class IfaceState
    {
        IFACE_STS_IDLE = 0,
        IFACE_STS_SCANNING
    };
    Q_ENUM_NS(IfaceState);

    QString ifaceStateToString(IfaceState idState);

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_QWLANTYPES_H
