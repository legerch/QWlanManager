#include "permissionwinrt.h"

#include <QDesktopServices>
#include <QUrl>

/*****************************/
/* Macro definitions         */
/*****************************/

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

/*****************************/
/* Functions implementation  */
/*****************************/

//TODO: add support for permissions signal changed
PermissionWinRt::PermissionWinRt(Permissions *parent)
    : PermissionsPrivate(parent)
{
    /* Nothing to do */
}

PermissionWinRt::~PermissionWinRt()
{
    /* Nothing to do */
}

void PermissionWinRt::initialize()
{
    m_permsWlan = WinRt::PermissionApp::Create(L"wiFiControl");
}

void PermissionWinRt::terminate()
{
    /* Nothing to do */
}

WlanPerm PermissionWinRt::updateStatus()
{
    const WinRt::PermissionId apiPerm = m_permsWlan.CheckAccess();
    setStatus(WinRt::convertPermFromApi(apiPerm));

    return m_currentPerm;
}

WlanError PermissionWinRt::prompt()
{
    qWarning("Permissions authorization prompt not available on WinRT platforms");
    return WlanError::WERR_OPERATION_UNSUPPORTED;
}

WlanError PermissionWinRt::openParams()
{
    const bool succeed = QDesktopServices::openUrl(QUrl("ms-settings:privacy-location"));
    if(!succeed){
        qWarning("Unable to open location privacy parameters");
        return WlanError::WERR_API_INTERNAL;
    }

    return WlanError::WERR_NO_ERROR;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
