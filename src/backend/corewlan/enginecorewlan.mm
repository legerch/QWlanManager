#include "enginecorewlan.h"

#import <CoreWLAN/CoreWLAN.h>

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

CWInterface* CoreWlan::getApiInterface(const Interface &interface)
{
    InterfaceMutator miface(interface);
    return std::any_cast<CWInterface*>(miface.getDataEngine());
}

CWNetwork* CoreWlan::getApiNetwork(const Network &network)
{
    NetworkMutator munet(network);
    return std::any_cast<CWNetwork*>(munet.getDataEngine());
}

WlanError CoreWlan::convertErrFromApi(const NSError *apiErr)
{
    /* Do error is set ? */
    if(!apiErr){
        return WlanError::WERR_NO_ERROR;
    }

    /* Do error came from CoreWlan API ? */
    if(![[apiErr domain] isEqualToString:CWErrorDomain]){
        logErrFromApi(apiErr);
        return WlanError::WERR_API_INTERNAL;
    }

    /* Convert code error to their equivalent */
    WlanError idErr = WlanError::WERR_API_INTERNAL;
    switch(apiErr.code)
    {
        case kCWNoErr:                      idErr = WlanError::WERR_NO_ERROR; break;

        case kCWInvalidParameterErr:
        case kCWInvalidFormatErr:           idErr = WlanError::WERR_ITEM_INVALID; break;

        case kCWNotSupportedErr:
        case kCWUnsupportedCapabilitiesErr: idErr = WlanError::WERR_OPERATION_UNSUPPORTED; break;

        case kCWTimeoutErr:                 idErr = WlanError::WERR_OPERATION_TIMEOUT; break;

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
    //TODO: add worker signals

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
    for(const CWInterface *apiIface : listInterfaces){
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

            miface.setDataEngine(std::any(apiIface));
        }

        m_currentIfaces.insert(iface.getUid(), iface);
    }
}

void EngineCoreWlan::interfaceScanNetworksAsync(Interface interface)
{

}

void EngineCoreWlan::interfaceConnectAsync(Interface interface, Network network, const QString &password)
{

}

void EngineCoreWlan::interfaceDisconnectAsync(Interface interface)
{

}

void EngineCoreWlan::interfaceForgetAsync(Interface interface, Network network)
{

}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
