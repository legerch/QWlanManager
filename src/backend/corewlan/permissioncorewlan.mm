#include "permissioncorewlan.h"

#import <CoreLocation/CLLocationManager.h>

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

WlanPerm PermissionCoreWlan::updateStatus()
{
    CLLocationManager *locManager = CoreWlan::getPermLocation(m_permsWlan);
    const CLAuthorizationStatus apiPerm = [locManager authorizationStatus];

    setStatus(CoreWlan::convertPermFromApi(apiPerm));

    return m_currentPerm;
}

WlanError PermissionCoreWlan::prompt()
{
    CLLocationManager *locManager = CoreWlan::getPermLocation(m_permsWlan);
    [locManager requestWhenInUseAuthorization];

    return WlanError::WERR_NO_ERROR;
}

WlanError PermissionCoreWlan::openParams()
{
    const bool succeed = QDesktopServices::openUrl(QUrl("x-apple.systempreferences:com.apple.preference.security?Privacy_LocationServices"));
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
