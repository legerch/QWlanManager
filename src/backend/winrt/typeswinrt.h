#ifndef QWLANMAN_BACKEND_WINRT_TYPESWINRT_H
#define QWLANMAN_BACKEND_WINRT_TYPESWINRT_H

#define _SILENCE_CLANG_COROUTINE_MESSAGE    1
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Security.Authorization.AppCapabilityAccess.h>

#include <QObject>

#include "qwlanmanager/qwlantypes.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm::WinRt
{
    Q_NAMESPACE

/*****************************/
/* Namespace aliases         */
/*****************************/
    namespace NsCapability          = winrt::Windows::Security::Authorization::AppCapabilityAccess;

/*****************************/
/* Types aliases             */
/*****************************/
    using PermissionId              = NsCapability::AppCapabilityAccessStatus;
    using PermissionApp             = NsCapability::AppCapability;

    using EventToken                = winrt::event_token;
    using EventArgsAccessChanged    = NsCapability::AppCapabilityAccessChangedEventArgs;

/*****************************/
/* Functions definitions     */
/*****************************/
    WlanPerm convertPermFromApi(PermissionId apiPerm);

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm::WinRt

#endif // QWLANMAN_BACKEND_WINRT_TYPESWINRT_H
