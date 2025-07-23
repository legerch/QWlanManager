#include "permissioncorewlan.h"

#import <CoreLocation/CoreLocation.h>

#include <QDebug>
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
/* Custom objective-C class  */
/*     LocationDelegate      */
/*        Definitions        */
/*****************************/

@interface LocationDelegate : NSObject<CLLocationManagerDelegate>
{
@private
    qwm::PermissionCoreWlan *m_permCoreWlan;
}

- (LocationDelegate*)init:(qwm::PermissionCoreWlan *)mpcw;

@end

/*****************************/
/* C++ Helper definitions    */
/*****************************/

namespace qwm::CoreWlan
{
    using CLLocationManagerPtr = std::shared_ptr<CLLocationManager>;
    using LocationDelegatePtr = std::shared_ptr<LocationDelegate>;

    CLLocationManagerPtr createApiPtrLocationManager();
    LocationDelegatePtr createApiptrLocationDelegate(PermissionCoreWlan *manager, CLLocationManager *apiManager);

    CLLocationManager* getLocationManager(const std::any &permLoc);

    WlanPerm convertPermFromApi(CLAuthorizationStatus apiPerm);

} // qwm::CoreWlan

/*****************************/
/* Custom objective-C class  */
/*     LocationDelegate      */
/*      Implementation       */
/*****************************/

@implementation LocationDelegate

- (LocationDelegate*)init:(qwm::PermissionCoreWlan *)mpcw
{
    self = [super init];

    m_permCoreWlan = mpcw;

    return self;
}

- (void)locationManagerDidChangeAuthorization:(CLLocationManager *)manager
{
    const qwm::WlanPerm idPerm = qwm::CoreWlan::convertPermFromApi(manager.authorizationStatus);
    m_permCoreWlan->setStatus(idPerm);
}

@end

/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm
{

/*****************************/
/* Helper implementations    */
/*****************************/

CoreWlan::CLLocationManagerPtr CoreWlan::createApiPtrLocationManager()
{
    CLLocationManager *apiPermLoc = [[CLLocationManager alloc] init];

    return std::shared_ptr<CLLocationManager>(apiPermLoc, [](CLLocationManager *ptr){
        [ptr release];
    });
}

CoreWlan::LocationDelegatePtr CoreWlan::createApiptrLocationDelegate(PermissionCoreWlan *manager, CLLocationManager *apiManager)
{
    /* Create instance */
    LocationDelegate *apiDelegate = [[LocationDelegate alloc] init:manager];

    /* Associate delegate to manager */
    apiManager.delegate = apiDelegate;

    /* Create shared pointer */
    return std::shared_ptr<LocationDelegate>(apiDelegate, [](LocationDelegate *ptr){
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

CLLocationManager* CoreWlan::getLocationManager(const std::any &permLoc)
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
    auto manager = CoreWlan::createApiPtrLocationManager();

    m_manager = manager;
    m_delegate = CoreWlan::createApiptrLocationDelegate(this, manager.get());
}

void PermissionCoreWlan::terminate()
{
    /* Nothing to do */
}

WlanPerm PermissionCoreWlan::updateStatus()
{
    CLLocationManager *locManager = CoreWlan::getLocationManager(m_manager);
    const CLAuthorizationStatus apiPerm = [locManager authorizationStatus];

    setStatus(CoreWlan::convertPermFromApi(apiPerm));

    return m_currentPerm;
}

WlanError PermissionCoreWlan::prompt()
{
    CLLocationManager *locManager = CoreWlan::getLocationManager(m_manager);
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
