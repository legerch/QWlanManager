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
