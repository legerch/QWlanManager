#include "enginewinnative.h"

#include <QDebug>

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

    /* Clean current list of interfaces */
    m_interfaces.clear();

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

        // Register an interface
        Interface iface = Interface::create();
        IfaceMutator miface(iface);

        miface.setUid(QUuid(pAdapter->AdapterName));
        miface.setHwAddress(pAdapter->PhysicalAddress, pAdapter->PhysicalAddressLength);
        miface.setName(QString::fromWCharArray(pAdapter->FriendlyName));
        miface.setDescription(desc);

        m_interfaces.insert(iface->getUid(), iface);
    }

stat_free:
    free(listAdapters);
    listAdapters = nullptr;

stat_return:
    return;
}

void EngineWinNative::interfaceScanNetworksAsync(Interface interface)
{
    /* Perform scan request */
    const GUID ifaceUuid = interface->getUid();
    DWORD res = WlanScan(m_handle, &ifaceUuid, nullptr, nullptr, nullptr);
    if(res != ERROR_SUCCESS){
        qCritical("Failed to perform scan request [err: %d]", res);
        emit q_ptr->sScanFailed(interface->getUid(), WlanError::WERR_API_INTERNAL);
        return;
    }

    /* Update interface state */
    IfaceMutator miface(interface);
    miface.setState(IfaceState::IFACE_STS_SCANNING);
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

void EngineWinNative::interfaceListUpdate()
{
    /* Move old list in order to keep a reference */
    MapInterfaces prevIfaces = std::move(m_interfaces);

    /* Refresh interface list */
    interfaceListRefresh();

    /* Perform comparaisons according to associated events */
    const auto newIds = QSet<QUuid>(m_interfaces.keyBegin(), m_interfaces.keyEnd());
    const auto oldIds = QSet<QUuid>(prevIfaces.keyBegin(), prevIfaces.keyEnd());

    /* Do interfaces has been added ? */
    const QSet<QUuid> setAdded = newIds - oldIds;
    for(auto it = setAdded.cbegin(); it != setAdded.cend(); ++it){
        emit q_ptr->sInterfaceAdded(m_interfaces.value(*it));
    }

    /* Do interfaces has been removed ? */
    const QSet<QUuid> setRemoved = oldIds - newIds;
    for(auto it = setRemoved.cbegin(); it != setRemoved.cend(); ++it){
        emit q_ptr->sInterfaceRemoved(prevIfaces.value(*it));
    }
}

//TODO: save signal quality information
//TODO: doc -> explain that hidden networks are currently ignored
WlanError EngineWinNative::interfaceNetworksUpdate(Interface interface)
{
    IfaceMutator miface(interface);
    MapNetworks &mapNets = miface.getMapNetworksRef();

    /* Clear current networks infos */
    miface.setConnectedSsid();
    mapNets.clear();

    /* Prepare arguments needed to perform request */
    constexpr DWORD flags = 0;
    const GUID ifaceUuid = interface->getUid();
    PWLAN_AVAILABLE_NETWORK_LIST apiNetList = nullptr;
    QString connectedSsid = ""; // Empty means "no network connected"

    /* Perform request */
    DWORD res = WlanGetAvailableNetworkList(m_handle, &ifaceUuid, flags, nullptr, &apiNetList);
    if(res != ERROR_SUCCESS){
        qCritical("Unable to retrieve list of networks [uuid: %s, err: %d]", qUtf8Printable(interface->getUid().toString()), res);
        return WlanError::WERR_API_INTERNAL;
    }

    /* Register networks */
    for(apiNetList->dwIndex = 0; apiNetList->dwIndex < apiNetList->dwNumberOfItems; ++apiNetList->dwIndex){
        PWLAN_AVAILABLE_NETWORK apiNet = &apiNetList->Network[apiNetList->dwIndex];

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

        // Register network
        Network net = Network::create();
        NetworkMutator munet(net);

        munet.setSsid(ssid);
        munet.setProfileName(profile);

        mapNets.insert(ssid, net);
    }

    /* Update interface connected network */
    miface.setConnectedSsid(connectedSsid);

    /* Clean used ressources */
    WlanFreeMemory(apiNetList);
    apiNetList = nullptr;

    return WlanError::WERR_NO_ERROR;
}

void EngineWinNative::interfaceScanFinished(const QUuid &idInterface, WlanError result)
{
    /* Retrive interface */
    Interface iface = m_interfaces.value(idInterface);

    /* Retrieve list of scanned networks */
    if(result == WlanError::WERR_NO_ERROR){
        result = interfaceNetworksUpdate(iface);
    }

    /* Reset interface state */
    IfaceMutator miface(iface);
    miface.setState(IfaceState::IFACE_STS_IDLE);

    /* Send associated signals */
    if(result == WlanError::WERR_NO_ERROR){
        emit q_ptr->sScanSucceed(idInterface, iface->getListNetworks());

    }else{
        qCritical("Scan request has failed [uuid: %s, err: %s (%d)]",
                  qUtf8Printable(idInterface.toString()),
                  qUtf8Printable(wlanErrorToString(result)),
                  result
        );

        emit q_ptr->sScanFailed(idInterface, result);
    }
}

/*!
 * \brief EngineWinNative::convertWinNativeErr
 * \param winErr
 *
 * \note
 * https://learn.microsoft.com/en-us/windows/win32/nativewifi/wlan-reason-code
 *
 * \return
 */
WlanError EngineWinNative::convertWinNativeErr(WLAN_REASON_CODE winErr)
{
    WlanError idErr = WlanError::WERR_NO_ERROR;

    switch(winErr)
    {
        case WLAN_REASON_CODE_USER_CANCELLED:                   idErr = WlanError::WERR_OPERATION_CANCEL;break;
        case WLAN_REASON_CODE_MSMSEC_CANCELLED:                 idErr = WlanError::WERR_OPERATION_CANCEL; break;

        case WLAN_REASON_CODE_NETWORK_NOT_AVAILABLE:            idErr = WlanError::WERR_NET_UNAVAILABLE; break;

        case WLAN_REASON_CODE_KEY_MISMATCH:                     idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PROFILE_KEY_LENGTH:        idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PROFILE_PSK_LENGTH:        idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PSK_MISMATCH_SUSPECTED:    idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_KEY_START_TIMEOUT:         idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_KEY_SUCCESS_TIMEOUT:       idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_KEY_FORMAT:                idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PROFILE_PASSPHRASE_CHAR:   idErr = WlanError::WERR_NET_PASSKEY; break;

        case WLAN_REASON_CODE_ASSOCIATION_TIMEOUT:              idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_SECURITY_TIMEOUT:                 idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_DISCONNECT_TIMEOUT:               idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_UI_REQUEST_TIMEOUT:               idErr = WlanError::WERR_OPERATION_TIMEOUT; break;

        default:                                                break;
    }

    return idErr;
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
        case wlan_notification_acm_scan_complete:{
            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);
            engine->interfaceScanFinished(idInterface, WlanError::WERR_NO_ERROR);
        }break;

        case wlan_notification_acm_scan_fail:{
            const QUuid idInterface = QUuid(ptrDataNotif->InterfaceGuid);

            WLAN_REASON_CODE *apiErr = static_cast<WLAN_REASON_CODE*>(ptrDataNotif->pData);
            const WlanError idErr = convertWinNativeErr(*apiErr);

            engine->interfaceScanFinished(idInterface, idErr);
        }break;

        case wlan_notification_acm_interface_arrival:
        case wlan_notification_acm_interface_removal:{
            engine->interfaceListUpdate();
        }break;
    }
}

void EngineWinNative::cbNotifMsm(QWLANMAN_VAR_UNUSED PWLAN_NOTIFICATION_DATA ptrDataNotif, QWLANMAN_VAR_UNUSED PVOID ptrDataCtx)
{
    // TODO: implement signal quality : wlan_notification_msm_signal_quality_change
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
