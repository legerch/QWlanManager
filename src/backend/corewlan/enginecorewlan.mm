#include "enginecorewlan.h"

extern "C" {  // Otherwise it won't find CWKeychain* symbols at link time
#   import <CoreWLAN/CoreWLAN.h>
}

#include "qwlanhelper.h"

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
/* Define constants          */
/*****************************/
const QUuid EngineCoreWlan::NS_UID = QUuid("{019812a3-ce07-7259-8f98-046be5b56d6e}");

/*****************************/
/* Helper definitions        */
/*****************************/

namespace CoreWlan
{
    using CWInterfacePtr = std::shared_ptr<CWInterface>;
    using CWNetworkPtr = std::shared_ptr<CWNetwork>;

    CWInterfacePtr makeApiPtrInterface(CWInterface *apiIface);
    CWNetworkPtr makeApiPtrNetwork(CWNetwork *apiNet);

    CWInterface* getApiInterface(const Interface &interface);
    CWNetwork* getApiNetwork(const Network &network);

    WlanError convertErrFromApi(const NSError *apiErr);
    void logErrFromApi(const NSError *apiErr);

    AuthAlgo convertAuthFromApi(CWSecurity apiAuth);
    AuthAlgo searchAuthFromNet(const CWNetwork *apiNet);

} // CoreWlan

/*****************************/
/* Helper implementations    */
/*****************************/

CoreWlan::CWInterfacePtr CoreWlan::makeApiPtrInterface(CWInterface *apiIface)
{
    return CWInterfacePtr([apiIface retain], [](CWInterface *ptr){
        [ptr release];
    });
}

CoreWlan::CWNetworkPtr CoreWlan::makeApiPtrNetwork(CWNetwork *apiNet)
{
    return CWNetworkPtr([apiNet retain], [](CWNetwork *ptr){
        [ptr release];
    });
}

CWInterface* CoreWlan::getApiInterface(const Interface &interface)
{
    InterfaceMutator miface(interface);

    CWInterfacePtr iface = std::any_cast<CWInterfacePtr>(miface.getDataEngine());
    return iface.get();
}

CWNetwork* CoreWlan::getApiNetwork(const Network &network)
{
    NetworkMutator munet(network);

    CWNetworkPtr net = std::any_cast<CWNetworkPtr>(munet.getDataEngine());
    return net.get();
}

WlanError CoreWlan::convertErrFromApi(const NSError *apiErr)
{
    /* Do error is set ? */
    if(!apiErr){
        return WlanError::WERR_NO_ERROR;
    }

    /* Convert code error to their equivalent */
    WlanError idErr = WlanError::WERR_API_INTERNAL;
    switch(apiErr.code)
    {
        case kCWNoErr:                      idErr = WlanError::WERR_NO_ERROR; break;

        case kCWInvalidFormatErr:           idErr = WlanError::WERR_ITEM_INVALID; break;

        case kCWNotSupportedErr:
        case kCWUnsupportedCapabilitiesErr: idErr = WlanError::WERR_OPERATION_UNSUPPORTED; break;

        case kCWTimeoutErr:                 idErr = WlanError::WERR_OPERATION_TIMEOUT; break;

        case kCWInvalidParameterErr:
        case kCWSupplicantTimeoutErr:       idErr = WlanError::WERR_NET_PASSKEY; break;

        default:                            logErrFromApi(apiErr); break;
    }

    return idErr;
}

void CoreWlan::logErrFromApi(const NSError *apiErr)
{
    const QString domain = QString::fromNSString(apiErr.domain);
    const QString desc = QString::fromNSString(apiErr.localizedDescription);
    const int idErr = static_cast<int>(apiErr.code);

    qWarning("Unable to convert NSError [domain: '%s', code: %d, description: '%s']", qUtf8Printable(domain), idErr, qUtf8Printable(desc));
}

AuthAlgo CoreWlan::convertAuthFromApi(CWSecurity apiAuth)
{
    AuthAlgo idAuth = AuthAlgo::AUTH_ALGO_UNKNOWN;

    switch(apiAuth)
    {
        case kCWSecurityNone:           idAuth = AuthAlgo::AUTH_ALGO_OPEN; break;

        case kCWSecurityWPA2Personal:
        case kCWSecurityPersonal:       idAuth = AuthAlgo::AUTH_ALGO_WPA2_PERSONAL; break;

        case kCWSecurityWPA2Enterprise:
        case kCWSecurityEnterprise:     idAuth = AuthAlgo::AUTH_ALGO_WPA2_ENTERPRISE; break;

        case kCWSecurityWPA3Personal:
        case kCWSecurityWPA3Transition: idAuth = AuthAlgo::AUTH_ALGO_WPA3_PERSONAL; break;

        case kCWSecurityWPA3Enterprise: idAuth = AuthAlgo::AUTH_ALGO_WPA3_ENTERPRISE; break;

        default:                        qWarning("Unable to convert authentication algorithm from API [id: %d]", static_cast<int>(apiAuth)); break;
    }

    return idAuth;
}

AuthAlgo CoreWlan::searchAuthFromNet(const CWNetwork *apiNet)
{
    /* Define list of tracked auth algorithms (order is important since a network can support multiple auth algos !) */
    static const QList<CWSecurity> TRACKED_AUTHS = {
        kCWSecurityNone,
        kCWSecurityWPA2Enterprise, kCWSecurityWPA2Personal,
        kCWSecurityWPA3Enterprise, kCWSecurityWPA3Personal, kCWSecurityWPA3Transition,
        kCWSecurityEnterprise, kCWSecurityPersonal
    };

    /* Find current security type */
    for(auto it = TRACKED_AUTHS.cbegin(); it != TRACKED_AUTHS.cend(); ++it){
        if([apiNet supportsSecurity:*it]){
            return convertAuthFromApi(*it);
        }
    }

    return AuthAlgo::AUTH_ALGO_UNKNOWN;
}

/*****************************/
/* Class definitions         */
/* HandleCoreWlan            */
/*****************************/

class HandleCoreWlan
{
public:
    static CWWiFiClient* instance();
};

/*****************************/
/* Functions implementation  */
/*      HandleCoreWlan       */
/*****************************/

CWWiFiClient* HandleCoreWlan::instance()
{
    static CWWiFiClient* client = [[CWWiFiClient sharedWiFiClient] retain];
    return client;
}

/*****************************/
/* Functions implementation  */
/*      WorkerCoreWlan       */
/*****************************/

WorkerCoreWlan::WorkerCoreWlan(QObject *parent)
    : QObject{parent}
{
    /* Nothing to do */
}

WorkerCoreWlan::~WorkerCoreWlan()
{
    /* Nothing to do */
}

void WorkerCoreWlan::performScan(const qwm::Interface &interface)
{
    /* Retrieve list of available networks */
    CWInterface *apiIface = CoreWlan::getApiInterface(interface);

    NSError *apiErr = nil;
    NSSet<CWNetwork*> *apiListNets = [apiIface scanForNetworksWithName:nil includeHidden:FALSE error:&apiErr];

    const WlanError scanResult = CoreWlan::convertErrFromApi(apiErr);
    if(scanResult != WlanError::WERR_NO_ERROR){
        qCritical("Failed to perform scan request [err: %d]", scanResult);
        emit sScanDone(interface, scanResult);
        return;
    }

    /* Prepare list of networks */
    InterfaceMutator miface(interface);
    MapNetworks &mapNets = miface.getMapNetworksRef();

    MapNetworks prevNets = mapNets;

    miface.setConnectedSsid();
    mapNets.clear();

    /* Retrieve list of known profiles (and put in cache) */
    const NSOrderedSet<CWNetworkProfile *> *apiListProfiles = [[apiIface configuration] networkProfiles];

    QSet<QString> cacheProfiles;
    for(const CWNetworkProfile *profile : apiListProfiles){
        const QString ssid = QString::fromNSString(profile.ssid);
        cacheProfiles.insert(ssid);
    }

    /* Register networks */
    const QDateTime now = QDateTime::currentDateTimeUtc();
    for(CWNetwork *apiNet : apiListNets){
        // Verify that network is infrastructure-based (is an access-point)
        if([apiNet ibss]){
            continue;
        }

        /*
         * Verify that network is not already registered
         * CoreWlan returns duplicated networks when SSID uses multiple
         * BSSIDs, so we have to filter them.
         * Also filter hidden networks
         */
        const QString ssid = QString::fromNSString(apiNet.ssid);
        if(ssid.isEmpty() || mapNets.contains(ssid)){
            continue;
        }

        // Set profile infos
        QString profile;
        if(cacheProfiles.contains(ssid)){
            profile = ssid;
        }

        // Manage known networks
        Network net;
        if(prevNets.contains(ssid)){
            net = prevNets.take(ssid);
        }

        // Update network properties
        NetworkMutator munet(net);

        munet.setSsid(ssid);
        munet.setProfileName(profile);
        munet.setAuthAlgo(CoreWlan::searchAuthFromNet(apiNet));
        munet.setCipherAlgo(CipherAlgo::CIPHER_ALGO_UNKNOWN);
        munet.setSignalQuality(Helper::calcSignalPercent(apiNet.rssiValue, apiNet.noiseMeasurement));

        munet.getCacheRef().markSeen(now);
        munet.setDataEngine(CoreWlan::makeApiPtrNetwork(apiNet));

        mapNets.insert(ssid, net);
    }

    /* Update interface properties */
    const QString currentSsid = QString::fromNSString(apiIface.ssid);

    miface.setConnectedSsid(currentSsid);
    miface.updateNetworksCached(prevNets, now);

    /* Worker has finished the scan request */
    emit sScanDone(interface, scanResult);
}

void WorkerCoreWlan::performConnect(const Interface &interface, const Network &network, const QString &passwd, qwm::WlanOptions opts)
{
    /* Prepare connection request arguments */
    CWInterface *apiIface = CoreWlan::getApiInterface(interface);
    CWNetwork *apiNet = CoreWlan::getApiNetwork(network);

    /* Do provided password is valid ? */
    NSString *apiPwd = nil;
    if(passwd.isEmpty()){
        // Do we allow to ask for admin access ?
        if(!opts.testFlag(WlanOption::WOPT_ALLOW_ADMIN_REQUESTS)){
            emit sConnectDone(interface, network, WlanError::WERR_NET_PASSKEY);
            return;
        }

        // Do we have a known network profile ?
        if(network.getProfileName().isEmpty()){
            emit sConnectDone(interface, network, WlanError::WERR_NET_PASSKEY);
            return;
        }

        // Retrieve password from system credential (require temporary admin access)
        OSStatus idResult = CWKeychainFindWiFiPassword(kCWKeychainDomainSystem, [apiNet ssidData], &apiPwd);
        if(idResult != errSecSuccess){
            qWarning("Unable to retrieve wifi credentials from keychain [id-err: 0x%08X]", idResult);
            emit sConnectDone(interface, network, WlanError::WERR_NET_PASSKEY);
            return;
        }

    /* Use an explicit password */
    }else{
        apiPwd = passwd.toNSString();
    }

    /* Perform connection request */
    NSError *apiErr = nil;
    bool succeed = [apiIface associateToNetwork:apiNet password:apiPwd error:&apiErr];
    if(!succeed){
        const WlanError idErr = CoreWlan::convertErrFromApi(apiErr);

        qCritical("Failed to perform connection request [err: %d]", idErr);
        emit sConnectDone(interface, network, idErr);
        return;
    }

    emit sConnectDone(interface, network, WlanError::WERR_NO_ERROR);
}

/*****************************/
/* Functions implementation  */
/*      EngineCoreWlan       */
/*****************************/

EngineCoreWlan::EngineCoreWlan(Manager *parent)
    : ManagerPrivate(parent)
{
    /* Nothing to do */
}

EngineCoreWlan::~EngineCoreWlan()
{
    /* Nothing to do */
}

void EngineCoreWlan::initialize()
{
    /* Create worker thread */
    m_worker = new WorkerCoreWlan();
    m_worker->moveToThread(&m_thread);

    QObject::connect(&m_thread, &QThread::finished, m_worker, &QObject::deleteLater);

    /* Connect worker signals */
    registerWorkerEvents();

    /* Start worker thread */
    m_thread.start();
}

void EngineCoreWlan::terminate()
{
    m_thread.quit();
    m_thread.wait();
}

void EngineCoreWlan::interfaceListRefresh()
{
    /* Retrieve available interfaces */
    CWWiFiClient *client = HandleCoreWlan::instance();
    NSArray<CWInterface *> *listInterfaces = [client interfaces];

    /* Parse each interface */
    for(CWInterface *apiIface : listInterfaces){
        // Manage known interfaces
        const QString hwAddr = QString::fromNSString([apiIface hardwareAddress]);
        const QUuid uid = QUuid::createUuidV5(NS_UID, hwAddr);

        Interface iface;
        if(m_prevIfaces.contains(uid)){
            iface = m_prevIfaces.value(uid);

        // Register an interface
        }else{
            InterfaceMutator miface(iface);

            miface.setUid(uid);
            miface.setHwAddress(hwAddr);
            miface.setName(QString::fromNSString([apiIface interfaceName]));
            miface.setDescription("");

            miface.setDataEngine(CoreWlan::makeApiPtrInterface(apiIface));
        }

        m_currentIfaces.insert(iface.getUid(), iface);
    }
}

void EngineCoreWlan::interfaceScanNetworksAsync(Interface interface)
{
    QMetaObject::invokeMethod(m_worker, &WorkerCoreWlan::performScan, Qt::QueuedConnection, interface);
}

void EngineCoreWlan::interfaceConnectAsync(Interface interface, Network network, const QString &password)
{
    QMetaObject::invokeMethod(m_worker, &WorkerCoreWlan::performConnect, Qt::QueuedConnection, interface, network, password, m_opts);
}

void EngineCoreWlan::interfaceDisconnectAsync(Interface interface)
{
    //TODO: implement
    handleDisconnectDone(interface, WlanError::WERR_OPERATION_UNSUPPORTED);
}

void EngineCoreWlan::interfaceForgetAsync(Interface interface, Network network)
{
    //TODO: implement
    handleForgetDone(interface, network, WlanError::WERR_OPERATION_UNSUPPORTED);
}

void EngineCoreWlan::registerWorkerEvents()
{
    QObject::connect(m_worker, &WorkerCoreWlan::sScanDone, q_ptr, [this](const Interface &interface, WlanError result){
        handleScanDone(interface, result);
    });

    QObject::connect(m_worker, &WorkerCoreWlan::sConnectDone, q_ptr, [this](const qwm::Interface &interface, const qwm::Network &network, qwm::WlanError result){
        handleConnectDone(interface, network.getSsid(), result);
    });
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
