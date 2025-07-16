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
    /* Prepare list of interfaces */
    MapInterfaces prevIfaces = m_interfaces;
    m_interfaces.clear();

    /* Retrieve available interfaces */
    CWWiFiClient *client = HandleCoreWlan::instance();
    NSArray<CWInterface *> *listInterfaces = [client interfaces];

    /* Parse each interface */
    for(const CWInterface *apiIface : listInterfaces){
        // Manage known interfaces
        const QString hwAddr = QString::fromNSString([apiIface hardwareAddress]);
        const QUuid uid = QUuid::createUuidV5(NS_UID, hwAddr);

        Interface iface;
        if(prevIfaces.contains(uid)){
            iface = prevIfaces.value(uid);

        // Register an interface
        }else{
            InterfaceMutator miface(iface);

            miface.setUid(uid);
            miface.setHwAddress(hwAddr);
            miface.setName(QString::fromNSString([apiIface interfaceName]));
            // Note: no interface description available

            miface.setDataEngine(std::any(apiIface));
        }

        m_interfaces.insert(iface.getUid(), iface);
    }

    /* Manage interface events */
    //TODO: move this part to manager private instance
    //interfaceListHandleEvents(prevIfaces, m_interfaces);
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
