#include "typeswinrt.h"

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
