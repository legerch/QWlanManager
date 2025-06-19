#include "enginewinnative.h"

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
 * \brief EngineWinNative::refreshInterfaces
 * \note
 * https://learn.microsoft.com/fr-fr/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses
 */
void EngineWinNative::refreshInterfaces()
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
        miface.setIsUp((pAdapter->OperStatus == IfOperStatusUp));

        m_interfaces.insert(iface->getUid(), iface);
    }

stat_free:
    free(listAdapters);
    listAdapters = nullptr;

stat_return:
    return;
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
    refreshInterfaces();

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

void EngineWinNative::cbNotifAcm(PWLAN_NOTIFICATION_DATA ptrDataNotif, PVOID ptrDataCtx)
{
    EngineWinNative *engine = static_cast<EngineWinNative*>(ptrDataCtx);

    switch(ptrDataNotif->NotificationCode)
    {
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
