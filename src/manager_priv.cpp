#include "manager_priv.h"

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

ManagerPrivate::ManagerPrivate(Manager *parent)
    : q_ptr(parent)
{
    /* Nothing to do */
}

ManagerPrivate::~ManagerPrivate()
{
    /* Nothing to do */
}

// TODO: track all isBusy (maybe allow to disable request behaviour)
void ManagerPrivate::interfaceScanNetworks(Interface interface)
{
    /* Verify that interface can perform a scan */
    if(interface.isBusy()){
        InterfaceMutator miface(interface);
        RequestsQueue &reqsQueue = miface.getRequestsQueueRef();

        reqsQueue.enqueue(Request::createReqScan());
        return;
    }

    /* Start scan request */
    interfaceScanNetworksAsync(interface);
}

//TODO: enqueue connection request (because, performing connection during scan is undefined behaviour but hard to be out this window !)
void ManagerPrivate::interfaceConnect(Interface interface, Network network, const QString &password)
{
    /* Verify that interface is not already connected */
    const Network current = interface.getNetworkConnected();
    if(interface.getNetworkConnected() == network){
        qDebug("Interface '%s' is already connected to '%s', nothing to perform", qUtf8Printable(interface.getName()), qUtf8Printable(network.getSsid()));

        emit q_ptr->sConnectionSucceed(interface.getUid(), network.getSsid());
        return;
    }

    /* Verify that interface can perform connection */
    if(interface.isBusy()){
        InterfaceMutator miface(interface);
        RequestsQueue &reqsQueue = miface.getRequestsQueueRef();

        reqsQueue.enqueue(Request::createReqConnect(network, password));
        return;
    }

    /* Start connection request */
    interfaceConnectAsync(interface, network, password);
}

void ManagerPrivate::interfaceDisconnect(Interface interface)
{
    /* Verify that interface is not already disconnected */
    const Network current = interface.getNetworkConnected();
    if(!current.isValid()){
        qDebug("Interface '%s' is already disconnected, nothing to perform", qUtf8Printable(interface.getName()));

        emit q_ptr->sDisconnectionSucceed(interface.getUid());
        return;
    }

    /* Verify that interface can perform disconnection */
    if(interface.isBusy()){
        InterfaceMutator miface(interface);
        RequestsQueue &reqsQueue = miface.getRequestsQueueRef();

        reqsQueue.enqueue(Request::createReqDisconnect());
        return;
    }

    /* Start disconnection request */
    interfaceDisconnectAsync(interface);
}

void ManagerPrivate::interfaceForget(Interface interface, Network network)
{
    /* Verify that network is not already forgotten */
    if(network.getProfileName().isEmpty()){
        qDebug("Network '%s' is already forgotten for interface '%s', nothing to perform", qUtf8Printable(network.getSsid()), qUtf8Printable(interface.getName()));

        emit q_ptr->sForgetSucceed(interface.getUid(), network.getSsid());
        return;
    }

    /* Start forgot request */
    interfaceForgetAsync(interface, network);
}

void ManagerPrivate::handleScanDone(const Interface &interface, WlanError idErr)
{
    requestHandleDone(interface, [this, &interface, idErr]
    {
        const QUuid idInterface = interface.getUid();

        if(idErr == WlanError::WERR_NO_ERROR){
            emit q_ptr->sScanSucceed(idInterface, interface.getListNetworks());
        }else{

            qCritical("Scan request has failed [uuid: %s, err: %s (%d)]",
                      qUtf8Printable(idInterface.toString()),
                      qUtf8Printable(wlanErrorToString(idErr)),
                      idErr
            );

            emit q_ptr->sScanFailed(idInterface, idErr);
        }
    });
}

void ManagerPrivate::handleConnectDone(const Interface &interface, const QString &ssid, WlanError idErr)
{
    requestHandleDone(interface, [this, &interface, &ssid, idErr]
    {
        const QUuid idInterface = interface.getUid();

        if(idErr == WlanError::WERR_NO_ERROR){
            InterfaceMutator miface(interface);
            miface.setConnectedSsid(ssid);

            emit q_ptr->sConnectionSucceed(idInterface, ssid);

        }else{
            qCritical("Connection request has failed [uuid: %s, ssid: %s, err: %s (%d)]",
                      qUtf8Printable(idInterface.toString()),
                      qUtf8Printable(ssid),
                      qUtf8Printable(wlanErrorToString(idErr)),
                      idErr
            );

            emit q_ptr->sConnectionFailed(idInterface, ssid, idErr);
        }
    });
}

void ManagerPrivate::handleDisconnectDone(const Interface &interface, WlanError idErr)
{
    requestHandleDone(interface, [this, &interface, idErr]
    {
        const QUuid idInterface = interface.getUid();

        if(idErr == WlanError::WERR_NO_ERROR){
            InterfaceMutator miface(interface);
            miface.setConnectedSsid();

            emit q_ptr->sDisconnectionSucceed(idInterface);

        }else{
            qCritical("Disconnection request has failed [uuid: %s, err: %s (%d)]",
                      qUtf8Printable(idInterface.toString()),
                      qUtf8Printable(wlanErrorToString(idErr)),
                      idErr
            );

            emit q_ptr->sDisconnectionFailed(idInterface, idErr);
        }
    });
}

void ManagerPrivate::requestPerform(const Interface &interface, const Request &req)
{
    switch(req.getType())
    {
    case RequestType::REQ_SCAN:{
        interfaceScanNetworks(interface);
    }break;

    case RequestType::REQ_CONNECT:{
        const QVariantMap data = req.getData().toMap();
        const Network net = data.value("network").value<Network>();
        const QString passwd =  data.value("password").toString();

        interfaceConnect(interface, net, passwd);
    }break;

    case qwm::RequestType::REQ_DISCONNECT:{
        interfaceDisconnect(interface);
    }break;

    default: break;
    }
}

void ManagerPrivate::requestHandleDone(const Interface &interface, std::function<void()> cbSignals)
{
    /* Reset interface state */
    InterfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_IDLE);

    /* Call associated signals */
    cbSignals();

    /* Do we have request in pending */
    RequestsQueue &reqsQueue = miface.getRequestsQueueRef();
    if(!reqsQueue.isEmpty()){
        auto req = reqsQueue.dequeue();
        if(req.has_value()){
            requestPerform(interface, req.value());
        }
    }
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
