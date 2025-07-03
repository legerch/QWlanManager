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

WlanPerm PermissionsMock::wlanRetrieve()
{
    qCritical("Unable to retrieve wlan permissions, not implemented for mock permissions");
    return WlanPerm::WPERM_UNKNOWN;
}

bool PermissionsMock::wlanOpenParams()
{
    qCritical("Unable to open wlan parameters, not implemented for mock permissions");
    return false;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
