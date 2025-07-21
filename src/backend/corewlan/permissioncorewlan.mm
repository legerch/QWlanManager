#include "permissioncorewlan.h"

#import <CoreLocation/CLLocationManager.h>

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
/* Helper definitions        */
/*****************************/

namespace CoreWlan
{
    using CLLocationManagerPtr = std::shared_ptr<CLLocationManager>;

    CLLocationManagerPtr createApiPtrPermLocation();
    CLLocationManager* getPermLocation(const std::any &permLoc);

    WlanPerm convertPermFromApi(CLAuthorizationStatus apiPerm);

} // CoreWlan

/*****************************/
/* Helper implementations    */
/*****************************/

CoreWlan::CLLocationManagerPtr CoreWlan::createApiPtrPermLocation()
{
    CLLocationManager *apiPermLoc = [[CLLocationManager alloc] init];

    return std::shared_ptr<CLLocationManager>(apiPermLoc, [](CLLocationManager *ptr){
        [ptr release];
    });
}

WlanPerm CoreWlan::convertPermFromApi(CLAuthorizationStatus apiPerm)
{
    WlanPerm idPerm = WlanPerm::WPERM_UNKNOWN;

    switch(apiPerm)
    {
        case kCLAuthorizationStatusNotDetermined:       idPerm = WlanPerm::WPERM_PROMPT_REQUIRED; break;

        case kCLAuthorizationStatusRestricted:
        case kCLAuthorizationStatusDenied:              idPerm = WlanPerm::WPERM_DENIED; break;

        case kCLAuthorizationStatusAuthorized:          idPerm = WlanPerm::WPERM_GRANTED; break;

        default: break;
    }

    return idPerm;
}

CLLocationManager* CoreWlan::getPermLocation(const std::any &permLoc)
{
    CLLocationManagerPtr apiLocManager = std::any_cast<CLLocationManagerPtr>(permLoc);
    return apiLocManager.get();
}

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
    m_permsWlan = CoreWlan::createApiPtrPermLocation();
}

void PermissionCoreWlan::terminate()
{
    /* Nothing to do */
}

WlanPerm PermissionCoreWlan::wlanRetrieve()
{
    CLLocationManager *locManager = CoreWlan::getPermLocation(m_permsWlan);
    const CLAuthorizationStatus apiPerm = [locManager authorizationStatus];

    m_currentPerm = CoreWlan::convertPermFromApi(apiPerm);

    return m_currentPerm;
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
