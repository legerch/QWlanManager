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

WlanPerm PermissionWinRt::wlanRetrieve()
{
    const WinRt::PermissionId apiPerm = m_permsWlan.CheckAccess();
    return WinRt::convertPermFromApi(apiPerm);
}

bool PermissionWinRt::wlanOpenParams()
{
    return QDesktopServices::openUrl(QUrl("ms-settings:privacy-location"));
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
