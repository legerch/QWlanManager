#include "permissionsmock.h"

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

PermissionsMock::PermissionsMock(Permissions *parent)
    : PermissionsPrivate(parent)
{
    qDebug("Permissions mock constructor called");
}

PermissionsMock::~PermissionsMock()
{
    qDebug("Permissions mock destructor called");
}

void PermissionsMock::initialize()
{
    qDebug("Permissions mock inititialization called");
}

void PermissionsMock::terminate()
{
    qDebug("Permissions mock termination called");
}

WlanPerm PermissionsMock::updateStatus()
{
    qCritical("Unable to retrieve wlan permissions, not implemented for mock permissions");
    return WlanPerm::WPERM_UNSUPPORTED;
}

WlanError PermissionsMock::prompt()
{
    qWarning("Permissions authorization prompt not available for mock permissions");
    return WlanError::WERR_OPERATION_UNSUPPORTED;
}

WlanError PermissionsMock::openParams()
{
    qWarning("Unable to open wlan parameters, not implemented for mock permissions");
    return WlanError::WERR_OPERATION_UNSUPPORTED;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
