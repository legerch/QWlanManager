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
    m_opts = WlanOption::WOPT_DEFAULT;
}

ManagerPrivate::~ManagerPrivate()
{
    /* Nothing to do */
}

/*!
 * \brief Update list of available interfaces
 * \details
 * Backends only report available interfaces (as new objects): already
 * known interfaces objects are reused here, so shared datas already
 * referenced by the application stay the same.
 */
void ManagerPrivate::interfaceListUpdate()
{
    /* Retrieve available interfaces */
    const ListInterfaces ifacesFound = interfaceListRetrieve();

    /* Register interfaces */
    MapInterfaces ifacesKnown = m_currentIfaces;

    MapInterfaces ifacesMap;
    ListInterfaces ifacesAdded;

    for(const Interface &found : ifacesFound){
        const QUuid uid = found.getUid();

        // Reuse known interface object (shared datas must stay the same)
        if(ifacesKnown.contains(uid)){
            ifacesMap.insert(uid, ifacesKnown.take(uid));
        }else{
            ifacesMap.insert(uid, found);
            ifacesAdded.append(found);
        }
    }

    /* Update list of interfaces */
    {
        QMutexLocker locker(&m_mutex);
        m_currentIfaces = ifacesMap;
    }

    /* Manage interfaces events (remaining known interfaces have been removed) */
    handleInterfacesListUpdateDone(ifacesAdded, ifacesKnown.values());
}

void ManagerPrivate::interfaceScanNetworks(Interface interface)
{
    /* Verify that interface can perform a scan */
    if(interface.isBusy()){
        if(interface.getOptions() & IFACE_OPT_REQUEST){
            requestAdd(interface, Request::createReqScan());
        }else{
            qWarning("Unable to perform scan, interface is busy [uuid: %s, state: %s (%d)]",
                     qUtf8Printable(interface.getUid().toString()),
                     qUtf8Printable(ifaceStateToString(interface.getState())),
                     QWLANMAN_E2I(interface.getState())
            );

            emit q_ptr->sScanFailed(interface.getUid(), WlanError::WERR_IFACE_BUSY);
        }
        return;
    }

    /* Update interface state */
    InterfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_SCANNING);

    /* Start scan request */
    interfaceScanNetworksAsync(interface);
}

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
        if(interface.getOptions() & IFACE_OPT_REQUEST){
            requestAdd(interface, Request::createReqConnect(network, password));
        }else{
            qWarning("Unable to perform connection, interface is busy [uuid: %s, state: %s (%d)]",
                     qUtf8Printable(interface.getUid().toString()),
                     qUtf8Printable(ifaceStateToString(interface.getState())),
                     QWLANMAN_E2I(interface.getState())
            );

            emit q_ptr->sConnectionFailed(interface.getUid(), network.getSsid(), WlanError::WERR_IFACE_BUSY);
        }
        return;
    }

    /* Update interface state */
    InterfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_CONNECTING);

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
        if(interface.getOptions() & IFACE_OPT_REQUEST){
            requestAdd(interface, Request::createReqDisconnect());
        }else{
            qWarning("Unable to perform disconnection, interface is busy [uuid: %s, state: %s (%d)]",
                     qUtf8Printable(interface.getUid().toString()),
                     qUtf8Printable(ifaceStateToString(interface.getState())),
                     QWLANMAN_E2I(interface.getState())
            );

            emit q_ptr->sDisconnectionFailed(interface.getUid(), WlanError::WERR_IFACE_BUSY);
        }
        return;
    }

    /* Update interface state */
    InterfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_DISCONNECTING);

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

/*!
 * \brief Emit signals related to interfaces list update
 *
 * \param[in] ifacesAdded
 * Interfaces added since previous update.
 * \param[in] ifacesRemoved
 * Interfaces removed since previous update.
 */
void ManagerPrivate::handleInterfacesListUpdateDone(const ListInterfaces &ifacesAdded, const ListInterfaces &ifacesRemoved)
{
    for(const Interface &iface : ifacesAdded){
        emit q_ptr->sInterfaceAdded(iface);
    }

    for(const Interface &iface : ifacesRemoved){
        emit q_ptr->sInterfaceRemoved(iface);
    }
}

/*!
 * \brief Manage end of a scan request
 * \details
 * Backends only report scan results: merging with known networks
 * and cache management are performed here.
 *
 * \param[in] interface
 * Interface which performed the scan.
 * \param[in] idErr
 * Scan result.
 * \param[in] connectedSsid
 * SSID of the network currently connected, empty if none. \n
 * Only used if scan succeed.
 * \param[in] listScans
 * Networks found by the scan. Must be new objects (not shared with
 * any other object). \n
 * Only used if scan succeed.
 */
void ManagerPrivate::handleScanDone(const Interface &interface, WlanError idErr, const QString &connectedSsid, const ListNetworks &listScans)
{
    /* Merge scan results */
    if(idErr == WlanError::WERR_NO_ERROR){
        interfaceNetworksMerge(interface, connectedSsid, listScans);
    }

    requestHandleDone(interface, [this, &interface, idErr]
    {
        const QUuid idInterface = interface.getUid();

        if(idErr == WlanError::WERR_NO_ERROR){
            emit q_ptr->sScanSucceed(idInterface, interface.getListNetworks());
        }else{

            qCritical("Scan request has failed [uuid: %s, err: %s (%d)]",
                      qUtf8Printable(idInterface.toString()),
                      qUtf8Printable(wlanErrorToString(idErr)),
                      QWLANMAN_E2I(idErr)
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
                      QWLANMAN_E2I(idErr)
            );

            emit q_ptr->sConnectionFailed(idInterface, ssid, idErr);
        }
    });
}

void ManagerPrivate::handleDisconnectDone(const Interface &interface, WlanError idErr)
{
    /* Do disconnection is due to a connection attempt ? */
    if(interface.getState() == IfaceState::IFACE_STS_CONNECTING){
        return;
    }

    /* Handle disconnection */
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
                      QWLANMAN_E2I(idErr)
            );

            emit q_ptr->sDisconnectionFailed(idInterface, idErr);
        }
    });
}

void ManagerPrivate::handleForgetDone(const Interface &interface, const Network &network, WlanError idErr)
{
    /* Do operation succeed ? */
    if(idErr != WlanError::WERR_NO_ERROR){
        emit q_ptr->sForgetFailed(interface.getUid(), network.getSsid(), idErr);
        return;
    }

    /* Update network informations */
    NetworkMutator munet(network);
    munet.setProfileName("");

    emit q_ptr->sForgetSucceed(interface.getUid(), network.getSsid());
}

/*!
 * \brief Merge scan results into interface networks
 * \details
 * This method must be called from the manager thread. \n
 * - Already known networks objects are reused (so shared datas already
 *   referenced by the application stay the same and see updated values),
 *   and their properties are updated from the scanned ones.
 * - Networks not seen during this scan are kept while their cache
 *   is not expired (see interface cache policy).
 *
 * \param[in] interface
 * Interface to update.
 * \param[in] connectedSsid
 * SSID of the network currently connected, empty if none.
 * \param[in] listScans
 * Networks found by the scan (new objects, not shared).
 */
void ManagerPrivate::interfaceNetworksMerge(const Interface &interface, const QString &connectedSsid, const ListNetworks &listScans)
{
    InterfaceMutator miface(interface);

    /* Prepare list of networks */
    MapNetworks prevNets = interface.getMapNetworks();
    MapNetworks mapNets;

    /* Register scanned networks */
    const QDateTime now = QDateTime::currentDateTimeUtc();
    for(const Network &scanned : listScans){
        const QString ssid = scanned.getSsid();

        // Reuse known network object (shared datas must stay the same)
        Network net = scanned;
        if(prevNets.contains(ssid)){
            net = prevNets.take(ssid);

            NetworkMutator munet(net);
            munet.updateFrom(scanned);
        }

        NetworkMutator munet(net);
        munet.getCacheRef().markSeen(now);

        mapNets.insert(ssid, net);
    }

    /* Keep unseen networks while their cache is not expired */
    const CachePolicy policy = interface.getCachePolicy();
    for(auto it = prevNets.cbegin(); it != prevNets.cend(); ++it){
        Network net = it.value();
        NetworkMutator munet(net);

        CacheInfo &cache = munet.getCacheRef();
        cache.markUnseen();

        if(cache.isExpired(policy, now)){
            continue;
        }

        mapNets.insert(it.key(), net);
    }

    /* Update interface networks */
    miface.setNetworks(mapNets, connectedSsid);
}

void ManagerPrivate::requestAdd(const Interface &interface, const Request &req)
{
    InterfaceMutator miface(interface);
    RequestsQueue &reqsQueue = miface.getRequestsQueueRef();

    reqsQueue.enqueue(req);
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
