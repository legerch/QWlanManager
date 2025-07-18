#include "permissioncorewlan.h"

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

PermissionCoreWlan::PermissionCoreWlan(Permissions *parent)
    : PermissionsPrivate(parent)
{
    /* Nothing to do */
}

PermissionCoreWlan::~PermissionCoreWlan()
{
    /* Nothing to do */
}

void PermissionCoreWlan::initialize()
{
    //TODO: implement
}

void PermissionCoreWlan::terminate()
{
    //TODO: implement
}

WlanPerm PermissionCoreWlan::wlanRetrieve()
{
    //TODO: implement
    return WlanPerm::WPERM_UNKNOWN;
}

bool PermissionCoreWlan::wlanOpenParams()
{
    //TODO: implement
    return false;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
