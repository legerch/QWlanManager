#include "typeswinrt.h"

/*****************************/
/* Namespace documentations  */
/*****************************/

/*!
 * \namespace qwm::WinRt
 * \brief Namespace used to manage types
 * from <b>Windows WinRT</b> API.
 * \details
 * For more details, see:
 * - https://learn.microsoft.com/fr-fr/uwp/api/windows.devices.wifi?view=winrt-26100
 * - https://learn.microsoft.com/fr-fr/uwp/api/windows.security.authorization.appcapabilityaccess?view=winrt-26100
 */

/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm::WinRt
{

WlanPerm convertPermFromApi(PermissionId apiPerm)
{
    WlanPerm idPerm = WlanPerm::WPERM_UNKNOWN;

    switch(apiPerm)
    {
        case PermissionId::NotDeclaredByApp:    idPerm = WlanPerm::WPERM_UNKNOWN; break;

        case PermissionId::UserPromptRequired:  idPerm = WlanPerm::WPERM_PROMPT_REQUIRED; break;

        case PermissionId::Allowed:             idPerm = WlanPerm::WPERM_GRANTED; break;

        case PermissionId::DeniedBySystem:
        case PermissionId::DeniedByUser:        idPerm = WlanPerm::WPERM_DENIED; break;

        default: break;
    }

    return idPerm;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm::WinRt

/*****************************/
/* End file                  */
/*****************************/
