#include "enginewinnative.h"

#include "profilewinnative.h"

#include <QDebug>

/*****************************/
/* Macro definitions         */
/*****************************/

/*!
 * Define the \c WLAN_REASON_CODE value associated
 * to a scan failure happening right after
 * passkeys used during a connection has been
 * exchanged
 *
 * \warning
 * This \c WLAN_REASON_CODE is not documented, this is
 * an internal error, which happens to be triggered
 * for this specific case. \n
 * So we need to track it.
 */
#define API_ERR_CODE_SCAN_FAIL_PASSKEY  (0xC001000C)

#define API_ERR_CODE_FMT                "0x%08X"

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

EngineWinNative::EngineWinNative(Manager *parent)
    : ManagerPrivate(parent)
{
    /* Nothing to do */
}

EngineWinNative::~EngineWinNative()
{
    /* Nothing to do */
}

void EngineWinNative::initialize()
{
    /* Open WLAN API */
    bool succeed = apiOpen();
    if(!succeed){
        return;
    }

    /* Register notifications events */
    eventRegister();
}

void EngineWinNative::terminate()
{
    eventUnregister();
    apiClose();
}

/*!
 * \brief EngineWinNative::interfaceListRefresh
 * \note
 * https://learn.microsoft.com/fr-fr/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses
 */
void EngineWinNative::interfaceListRefresh()
{
    /* Prepare arguments used to retrieve interfaces */
    constexpr int MAX_TRIALS = 3;
    ULONG bufSize = 15000;
    ULONG flags = GAA_FLAG_SKIP_DNS_SERVER | GAA_FLAG_SKIP_MULTICAST;

    PIP_ADAPTER_ADDRESSES listAdapters = nullptr;
    DWORD res = 0;
    int nbTrials = 0;

    /* Retrieve interfaces list */
    do{
        // Allocate memory fo adapter list
        listAdapters = (IP_ADAPTER_ADDRESSES *)malloc(bufSize);
        if(!listAdapters){
            qCritical("Failed to allocate memory for adapters list [buffer-size: %ul]", bufSize);
            goto stat_return;
        }

        // Perform request
        res = GetAdaptersAddresses(AF_UNSPEC, flags, nullptr, listAdapters, &bufSize);
        if(res == ERROR_BUFFER_OVERFLOW){
            free(listAdapters);
            listAdapters = nullptr;
            qWarning("Buffer size too small for adapters list [better-size: %ul, nb-trials: %d]", bufSize, nbTrials);
        }

        ++nbTrials;

    }while(res == ERROR_BUFFER_OVERFLOW && nbTrials < MAX_TRIALS);

    /* Verify status */
    if(res != ERROR_SUCCESS){
        qCritical("Failed to retrieve adapters list [err: %d]", res);
        goto stat_free;
    }

    /* Store interfaces informations */
    for(PIP_ADAPTER_ADDRESSES pAdapter = listAdapters; pAdapter; pAdapter = pAdapter->Next){
        // Verify that adapter is a wireless IEEE 802.11 interface
        if(pAdapter->IfType != IF_TYPE_IEEE80211){
            continue;
        }

        // Verify that interface is not virtual
        const QString desc = QString::fromWCharArray(pAdapter->Description);
        if(interfaceIsVirtual(desc)){
            continue;
        }

        // Manage known interface
        const QUuid uid = QUuid(pAdapter->AdapterName);

        Interface iface;
        if(m_prevIfaces.contains(uid)){
            iface = m_prevIfaces.value(uid);

        // Register an interface
        }else{
            InterfaceMutator miface(iface);

            miface.setUid(uid);
            miface.setHwAddress(pAdapter->PhysicalAddress, pAdapter->PhysicalAddressLength);
            miface.setName(QString::fromWCharArray(pAdapter->FriendlyName));
            miface.setDescription(desc);
        }

        m_currentIfaces.insert(iface.getUid(), iface);
    }

stat_free:
    free(listAdapters);
    listAdapters = nullptr;

stat_return:
    return;
}

/*!
 * \brief EngineWinNative::interfaceScanNetworksAsync
 * \param interface
 *
 * \note
 * https://learn.microsoft.com/fr-fr/windows/win32/api/wlanapi/nf-wlanapi-wlanscan
 */
void EngineWinNative::interfaceScanNetworksAsync(Interface interface)
{
    /* Perform scan request */
    const GUID ifaceUuid = interface.getUid();
    const DWORD res = WlanScan(m_handle, &ifaceUuid, nullptr, nullptr, nullptr);
    if(res != ERROR_SUCCESS){
        qCritical("Failed to perform scan request [err: %d]", res);
        emit q_ptr->sScanFailed(interface.getUid(), WlanError::WERR_API_INTERNAL);
        return;
    }

    /* Update interface state */
    InterfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_SCANNING);
}

void EngineWinNative::interfaceConnectAsync(Interface interface, Network network, const QString &password)
{
    /* Do profile must be created/updated ? */
    if(!password.isEmpty()){
        const WlanError errProfile = interfaceNetworkProfileCreate(interface, network, password);
        if(errProfile != WlanError::WERR_NO_ERROR){
            emit q_ptr->sConnectionFailed(interface.getUid(), network.getSsid(), errProfile);
            return;
        }
    }

    /* Do we have a valid profile ? */
    const QString &profileName = network.getProfileName();
    if(profileName.isEmpty()){
        emit q_ptr->sConnectionFailed(interface.getUid(), network.getSsid(), WlanError::WERR_NET_PASSKEY);
        return;
    }

    /* Prepare request arguments */
    const GUID ifaceUuid = interface.getUid();
    const std::wstring wstrProfile = profileName.toStdWString();
    const LPCWSTR strProfile = static_cast<LPCWSTR>(wstrProfile.c_str());

    WLAN_CONNECTION_PARAMETERS params;
    params.wlanConnectionMode = wlan_connection_mode_profile;
    params.strProfile = strProfile;
    params.pDot11Ssid = nullptr;
    params.pDesiredBssidList = nullptr;
    params.dot11BssType = dot11_BSS_type_infrastructure;
    params.dwFlags = 0;

    PWLAN_CONNECTION_PARAMETERS ptrParams = &params;

    /* Perform connection request */
    const DWORD res = WlanConnect(m_handle, &ifaceUuid, ptrParams, nullptr);
    if(res != ERROR_SUCCESS){
        qCritical("Failed to perform connection request [err: %d]", res);
        emit q_ptr->sConnectionFailed(interface.getUid(), network.getSsid(), WlanError::WERR_API_INTERNAL);
        return;
    }

    /* Update interface state */
    InterfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_CONNECTING);
}

void EngineWinNative::interfaceDisconnectAsync(Interface interface)
{
    /* Perform disconnection request */
    const GUID ifaceUuid = interface.getUid();
    const DWORD res = WlanDisconnect(m_handle, &ifaceUuid, nullptr);
    if(res != ERROR_SUCCESS){
        qCritical("Failed to perform disconnection request [err: %d]", res);
        emit q_ptr->sDisconnectionFailed(interface.getUid(), WlanError::WERR_API_INTERNAL);
        return;
    }

    /* Update interface state */
    InterfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_DISCONNECTING);
}

void EngineWinNative::interfaceForgetAsync(Interface interface, Network network)
{
    /* Forget network */
    const WlanError idErr = interfaceNetworkProfileDelete(interface, network);
    handleForgetDone(interface, network, idErr);
}

bool EngineWinNative::apiOpen()
{
    DWORD verAsked = WLAN_API_VERSION;
    DWORD verReturned = 0;

    DWORD res = WlanOpenHandle(verAsked, NULL, &verReturned, &m_handle);
    if(res != ERROR_SUCCESS){
        qCritical("Unable to open WLAN API [err: %d, version-asked: %d, version-returned: %d]", res, verAsked, verReturned);
        return false;
    }

    return true;
}

void EngineWinNative::apiClose()
{
    DWORD res = WlanCloseHandle(m_handle, NULL);
    if(res != ERROR_SUCCESS){
        qCritical("Unable to close WLAN API [err: %d]", res);
        return;
    }
}

/*!
 * \brief EngineWinNative::eventRegister
 * \return
 *
 * \note
 * https://learn.microsoft.com/fr-fr/windows/win32/api/wlanapi/nf-wlanapi-wlanregisternotification
 */
bool EngineWinNative::eventRegister()
{
    DWORD res = WlanRegisterNotification(m_handle, WLAN_NOTIFICATION_SOURCE_ACM | WLAN_NOTIFICATION_SOURCE_MSM, true,
                                         &EngineWinNative::cbNotif, this, nullptr, nullptr
    );

    if(res != ERROR_SUCCESS){
        qCritical("Unable to register for WLAN notifications events [err: %d]", res);
        return false;
    }

    return true;
}

void EngineWinNative::eventUnregister()
{
    DWORD res = WlanRegisterNotification(m_handle, WLAN_NOTIFICATION_SOURCE_NONE, false,
                                         nullptr, this, nullptr, nullptr
                                         );

    if(res != ERROR_SUCCESS){
        qCritical("Unable to unregister WLAN notifications events [err: %d]", res);
        return;
    }
}

/*!
 * \brief Use to known if device adapter is virtual or not
 * \details
 * Use interface description to get this information
 * since native APIs doesn't provide those informations
 * without reading registry directly.
 *
 * \param[in] description
 * Interface description to verify.
 *
 * \return
 * Return \c true is adapter is a virtual adapter
 */
bool EngineWinNative::interfaceIsVirtual(const QString &description)
{
    return description.contains("virtual", Qt::CaseInsensitive)
        || description.contains("vmware", Qt::CaseInsensitive);
}

WlanError EngineWinNative::interfaceNetworksUpdate(Interface interface)
{
    InterfaceMutator miface(interface);
    MapNetworks &mapNets = miface.getMapNetworksRef();

    /* Prepare list of networks */
    MapNetworks prevNets = mapNets;

    miface.setConnectedSsid();
    mapNets.clear();

    /* Prepare arguments needed to perform request */
    constexpr DWORD flags = 0;
    const GUID ifaceUuid = interface.getUid();
    PWLAN_AVAILABLE_NETWORK_LIST apiNetList = nullptr;
    QString connectedSsid = ""; // Empty means "no network connected"

    /* Perform request */
    DWORD res = WlanGetAvailableNetworkList(m_handle, &ifaceUuid, flags, nullptr, &apiNetList);
    if(res != ERROR_SUCCESS){
        qCritical("Unable to retrieve list of networks [uuid: %s, err: %d]", qUtf8Printable(interface.getUid().toString()), res);
        return WlanError::WERR_API_INTERNAL;
    }

    /* Register networks */
    const QDateTime now = QDateTime::currentDateTimeUtc();
    for(apiNetList->dwIndex = 0; apiNetList->dwIndex < apiNetList->dwNumberOfItems; ++apiNetList->dwIndex){
        PWLAN_AVAILABLE_NETWORK apiNet = &apiNetList->Network[apiNetList->dwIndex];

        // Verify that network is infrastructure-based (is an access-point)
        if(apiNet->dot11BssType != dot11_BSS_type_infrastructure){
            continue;
        }

        /*
         * Verify that network is not already registered
         * WlanAPI returns duplicated networks when a profile already exists,
         * so we have to filter them.
         * Also filter hidden networks
         */
        const QString ssid = QString::fromUtf8(reinterpret_cast<char *>(apiNet->dot11Ssid.ucSSID), apiNet->dot11Ssid.uSSIDLength);
        if(ssid.isEmpty() || mapNets.contains(ssid)){
            continue;
        }

        // Parse profile infos
        QString profile;
        if(apiNet->dwFlags & WLAN_AVAILABLE_NETWORK_HAS_PROFILE){
            profile = QString::fromWCharArray(apiNet->strProfileName);
        }

        // Determine if current network
        if(apiNet->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED){
            connectedSsid = ssid;
        }

        // Manage known network
        Network net;
        if(prevNets.contains(ssid)){
            net = prevNets.take(ssid);
        }

        // Update network properties
        NetworkMutator munet(net);

        munet.setSsid(ssid);
        munet.setProfileName(profile);
        munet.setAuthAlgo(WinNative::convertAuthFromApi(apiNet->dot11DefaultAuthAlgorithm));
        munet.setCipherAlgo(WinNative::convertCipherFromApi(apiNet->dot11DefaultCipherAlgorithm));
        munet.setSignalQuality(apiNet->wlanSignalQuality);

        munet.getCacheRef().markSeen(now);

        mapNets.insert(ssid, net);
    }

    /* Update interface properties */
    miface.setConnectedSsid(connectedSsid);
    miface.updateNetworksCached(prevNets, now);

    /* Clean used ressources */
    WlanFreeMemory(apiNetList);
    apiNetList = nullptr;

    return WlanError::WERR_NO_ERROR;
}

/*!
 * \brief EngineWinNative::interfaceNetworkCreateProfile
 * \param interface
 * \param network
 * \param password
 *
 * \note
 * https://learn.microsoft.com/fr-fr/windows/win32/api/wlanapi/nf-wlanapi-wlansetprofile
 *
 * \return
 */
WlanError EngineWinNative::interfaceNetworkProfileCreate(Interface interface, Network network, const QString &password)
{
    /* Prepare profile XML */
    const ProfileWinNative profile(network, password);
    const QString xmlProfile = profile.toXmlFormat();

    /* Prepare argument request */
    constexpr DWORD flags = 0;
    const GUID ifaceUuid = interface.getUid();
    const std::wstring wstrProfile = xmlProfile.toStdWString();
    const LPCWSTR strProfile = static_cast<LPCWSTR>(wstrProfile.c_str());

    WLAN_REASON_CODE idStatus = WLAN_REASON_CODE_UNKNOWN;

    /* Perform request */
    const DWORD res = WlanSetProfile(m_handle, &ifaceUuid, flags, strProfile, nullptr, true, nullptr, &idStatus);
    if(res != ERROR_SUCCESS){
        qCritical("Unable to set profile [uuid: %s, ssid: %s, id-result: %d, id-reason: " API_ERR_CODE_FMT "]",
                  qUtf8Printable(interface.getUid().toString()),
                  qUtf8Printable(network.getSsid()),
                  res, idStatus
        );
        return WinNative::convertErrWlanFromApi(idStatus);
    }

    /* Update network informations */
    NetworkMutator munet(network);
    munet.setProfileName(profile.getName());

    return WlanError::WERR_NO_ERROR;
}

WlanError EngineWinNative::interfaceNetworkProfileDelete(Interface interface, Network network)
{
    /* Prepare profile informations */
    const ProfileWinNative profile(network, QString());
    const QString profileApi = profile.getNameFmt();

    /* Prepare argument request */
    const GUID ifaceUuid = interface.getUid();
    const std::wstring wstrProfile = profileApi.toStdWString();
    const LPCWSTR strProfile = static_cast<LPCWSTR>(wstrProfile.c_str());

    /* Perform request */
    const DWORD res = WlanDeleteProfile(m_handle, &ifaceUuid, strProfile, nullptr);
    if(res != ERROR_SUCCESS){
        qCritical("Unable to delete profile [uuid: %s, ssid: %s, id-result: %d]",
                  qUtf8Printable(interface.getUid().toString()),
                  qUtf8Printable(network.getSsid()),
                  res
        );
        return WlanError::WERR_API_INTERNAL;
    }

    return WlanError::WERR_NO_ERROR;
}

void EngineWinNative::interfaceScanFinished(const QUuid &idInterface, WlanError result)
{
    /* Retrieve interface */
    Interface iface = m_currentIfaces.value(idInterface);

    /* Retrieve list of scanned networks */
    if(result == WlanError::WERR_NO_ERROR){
        result = interfaceNetworksUpdate(iface);
    }

    /* Manage end of request */
    handleScanDone(iface, result);
}

void EngineWinNative::interfaceConnectionFinished(const QUuid &idInterface, const QString &ssid, WlanError result)
{
    Interface iface = m_currentIfaces.value(idInterface);
    handleConnectDone(iface, ssid, result);
}

void EngineWinNative::interfaceDisconnectionFinished(const QUuid &idInterface, WlanError result)
{
    Interface iface = m_currentIfaces.value(idInterface);
    handleDisconnectDone(iface, result);
}

void EngineWinNative::interfaceSignalQualityReceived(const QUuid &idInterface, uint percent)
{
    /* Retrieve associated interface */
    Interface iface = m_currentIfaces.value(idInterface);
    if(!iface.isValid()){
        qWarning("Unable to update signal quality, invalid interface [uuid: %s]", qUtf8Printable(idInterface.toString()));
        return;
    }

    /* Retrieve current network */
    Network net = iface.getNetworkConnected();
    if(!net.isValid()){
        qWarning("Unable to update signal quality, current network not set [iface-uuid: %s, iface-name: '%s']",
                qUtf8Printable(idInterface.toString()),
                qUtf8Printable(iface.getName())
        );
        return;
    }

    /* Update network informations */
    NetworkMutator munet(net);
    munet.setSignalQuality(percent);

    /* Emit associated signals */
    emit q_ptr->sSignalQualityChanged(idInterface, percent);
}

/*!
 * \brief Notification callback function
 * \details
 * This function is used each time a notification from WlanAPI is received.
 *
 * \param[in] ptrDataNotif
 * Pointer to a \b WLAN_NOTIFICATION_DATA structure that contains notification informations.
 * \param[in, out] ptrDataCtx
 * Pointer to context information provided by the client when it registered for notifications.
 *
 * \note
 * For developers:
 * - https://docs.microsoft.com/en-us/windows/win32/api/wlanapi/nc-wlanapi-wlan_notification_callback
 *
 * \sa cbNotifAcm(), cbNotifMsm()
 */
void EngineWinNative::cbNotif(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx)
{
    switch(ptrDataNotif->NotificationSource)
    {
        case WLAN_NOTIFICATION_SOURCE_ACM:  cbNotifAcm(ptrDataNotif, ptrDataCtx); break;
        case WLAN_NOTIFICATION_SOURCE_MSM:  cbNotifMsm(ptrDataNotif, ptrDataCtx); break;

        default: break;
    }
}

/*!
 * \brief EngineWinNative::cbNotifAcm
 * \param ptrDataNotif
 * \param ptrDataCtx
 *
 * \note
 * https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms706902(v=vs.85)
 */
void EngineWinNative::cbNotifAcm(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx)
{
    EngineWinNative *engine = static_cast<EngineWinNative*>(ptrDataCtx);

    switch(ptrDataNotif->NotificationCode)
    {
        case wlan_notification_acm_interface_arrival:
        case wlan_notification_acm_interface_removal:{
            engine->interfaceListUpdate();
        }break;

        case wlan_notification_acm_scan_complete:{
            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);
            engine->interfaceScanFinished(idInterface, WlanError::WERR_NO_ERROR);
        }break;

        case wlan_notification_acm_scan_fail:{
            WLAN_REASON_CODE *apiErr = static_cast<WLAN_REASON_CODE*>(ptrDataNotif->pData);

            /* Do we have a scan failure due to passkey exchanged */
            if(*apiErr == API_ERR_CODE_SCAN_FAIL_PASSKEY){
                qDebug("Scan failure has been ignored, triggered by a passkey exchange during connection");
                goto stat_return;
            }

            /* Manage scan failure */
            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);
            const WlanError idErr = WinNative::convertErrWlanFromApi(*apiErr);

            engine->interfaceScanFinished(idInterface, idErr);
        }break;

        case  wlan_notification_acm_connection_attempt_fail:{
            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);
            const PWLAN_CONNECTION_NOTIFICATION_DATA connectData = static_cast<PWLAN_CONNECTION_NOTIFICATION_DATA>(ptrDataNotif->pData);

            engine->m_errConnect.insert(idInterface, connectData->wlanReasonCode);
        }break;

        case wlan_notification_acm_connection_complete:{
            const PWLAN_CONNECTION_NOTIFICATION_DATA connectData = static_cast<PWLAN_CONNECTION_NOTIFICATION_DATA>(ptrDataNotif->pData);

            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);
            const QString ssid = QString::fromUtf8(reinterpret_cast<char *>(connectData->dot11Ssid.ucSSID), connectData->dot11Ssid.uSSIDLength);

            const WLAN_REASON_CODE apiErr = engine->m_errConnect.value(idInterface, connectData->wlanReasonCode);
            const WlanError idErr = WinNative::convertErrWlanFromApi(apiErr);
            engine->m_errConnect.remove(idInterface);

            engine->interfaceConnectionFinished(idInterface, ssid, idErr);
        }break;

        case wlan_notification_acm_disconnected:{
            const PWLAN_CONNECTION_NOTIFICATION_DATA connectData = static_cast<PWLAN_CONNECTION_NOTIFICATION_DATA>(ptrDataNotif->pData);

            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);
            const WlanError idErr = WinNative::convertErrWlanFromApi(connectData->wlanReasonCode);

            engine->interfaceDisconnectionFinished(idInterface, idErr);
        }break;

        default: break;
    }

stat_return:;
}

void EngineWinNative::cbNotifMsm(QWLANMAN_VAR_UNUSED PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx)
{
    EngineWinNative *engine = static_cast<EngineWinNative*>(ptrDataCtx);

    switch(ptrDataNotif->NotificationCode)
    {
        case wlan_notification_msm_signal_quality_change:{
            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);
            const ULONG *apiQuality = static_cast<ULONG *>(ptrDataNotif->pData);

            engine->interfaceSignalQualityReceived(idInterface, *apiQuality);
        }break;

        default: break;
    }
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
