#include "enginenetworkmanager.h"

#include "deviceinfoprovider.h"
#include "typesnm.h"

#include <QDBusInterface>
#include <QDBusReply>

/*****************************/
/* Macro definitions         */
/*****************************/

#define DBUS_SERVICE            "org.freedesktop.NetworkManager"
#define DBUS_PATH_MANAGER       "/org/freedesktop/NetworkManager"

#define DBUS_INTERFACE_MANAGER  DBUS_SERVICE
#define DBUS_INTERFACE_DEVICE   DBUS_INTERFACE_MANAGER ".Device"

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

//TODO: check NMClientPermission : https://networkmanager.dev/docs/api/latest/nm-dbus-types.html
//TODO: to add in doc: qdbus --system org.freedesktop.NetworkManager /org/freedesktop/NetworkManager org.freedesktop.DBus.Introspectable.Introspect

/*****************************/
/* Constants definitions     */
/*****************************/

const QUuid EngineNetworkManager::NM_UID = QUuid("{0e6a5c93-c7df-4973-ae6b-81ed3f41339c}");

/*****************************/
/* Functions implementation  */
/*  DelegateNetworkManager   */
/*****************************/

DelegateNetworkManager::DelegateNetworkManager(EngineNetworkManager *engine)
    : QObject(), m_engine(engine)
{
    /* Nothing to do */
}

DelegateNetworkManager::~DelegateNetworkManager()
{
    /* Nothing to do */
}

void DelegateNetworkManager::handleDeviceAdded(QWLANMAN_VAR_UNUSED const QDBusMessage &msg)
{
    m_engine->interfaceListUpdate();
}

void DelegateNetworkManager::handleDeviceRemoved(QWLANMAN_VAR_UNUSED const QDBusMessage &msg)
{
    m_engine->interfaceListUpdate();
}

void DelegateNetworkManager::eventRegister()
{
    QDBusConnection apiBus = QDBusConnection::systemBus();

    /* Register interface added/remove events */
    apiBus.connect(DBUS_SERVICE, DBUS_PATH_MANAGER, DBUS_INTERFACE_MANAGER, "DeviceAdded", this, SLOT(handleDeviceAdded(QDBusMessage)));
    apiBus.connect(DBUS_SERVICE, DBUS_PATH_MANAGER, DBUS_INTERFACE_MANAGER, "DeviceRemoved", this, SLOT(handleDeviceRemoved(QDBusMessage)));
}

void DelegateNetworkManager::eventUnregister()
{
    QDBusConnection apiBus = QDBusConnection::systemBus();

    /* Unregister interface added/remove events */
    apiBus.disconnect(DBUS_SERVICE, DBUS_PATH_MANAGER, DBUS_INTERFACE_MANAGER, "DeviceAdded", this, SLOT(handleDeviceAdded(QDBusMessage)));
    apiBus.disconnect(DBUS_SERVICE, DBUS_PATH_MANAGER, DBUS_INTERFACE_MANAGER, "DeviceRemoved", this, SLOT(handleDeviceRemoved(QDBusMessage)));
}

/*****************************/
/* Functions implementation  */
/*   EngineNetworkManager    */
/*****************************/

EngineNetworkManager::EngineNetworkManager(Manager *parent)
    : ManagerPrivate(parent), m_delegate(this)
{
    /* Nothing to do */
}

EngineNetworkManager::~EngineNetworkManager()
{
    /* Nothing to do */
}

void EngineNetworkManager::initialize()
{
    /* Register notifications events */
    m_delegate.eventRegister();
}

void EngineNetworkManager::terminate()
{
    m_delegate.eventUnregister();
}

void EngineNetworkManager::interfaceListRefresh()
{
    /* Create DBus interface for network manager */
    QDBusInterface busNm(DBUS_SERVICE, DBUS_PATH_MANAGER, DBUS_INTERFACE_MANAGER, QDBusConnection::systemBus());
    if(!busNm.isValid()){
        qCritical("Network manager not available");
        return;
    }

    /* Retrieve list of network devices */
    QDBusReply<QList<QDBusObjectPath>> reply = busNm.call("GetDevices");
    if(!reply.isValid()) {
        qCritical("Failed to retrieve list of interfaces [err: '%s']", qUtf8Printable(reply.error().message()));
        return;
    }

    /* Parse each interface */
    const QList<QDBusObjectPath> devicePaths = reply.value();
    for(const QDBusObjectPath &devPath : devicePaths){
        // Set a bus interface for device
        const QDBusInterface busDev(DBUS_SERVICE, devPath.path(), DBUS_INTERFACE_DEVICE, QDBusConnection::systemBus());
        if(!busDev.isValid()){
            continue;
        }

        // Verify that adapter is a wireless IEEE 802.11 interface
        const int devType = busDev.property("DeviceType").toInt();
        if(!qwm::NmBus::isWifiDevice(devType)){
            continue;
        }

        // Is it a real device ?
        const bool devIsReal = busDev.property("Real").toBool();
        if(!devIsReal){
            continue;
        }

        // Generate UID
        const QString name = busDev.property("Interface").toString();
        const QString hwAddr = busDev.property("HwAddress").toString();

        const QUuid uid = QUuid::createUuidV5(NM_UID, hwAddr);

        // Manage known interface
        Interface iface;
        if(m_prevIfaces.contains(uid)){
            iface = m_prevIfaces.value(uid);

        // Register an interface
        }else{
            InterfaceMutator miface(iface);

            miface.setUid(uid);
            miface.setHwAddress(hwAddr);
            miface.setName(name);
            miface.setDescription(DeviceInfosProvider::retrieveAdapterDesc(name));
        }

        m_currentIfaces.insert(iface.getUid(), iface);
    }
}

void EngineNetworkManager::interfaceScanNetworksAsync(Interface interface)
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceConnectAsync(Interface interface, Network network, const QString &password)
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceDisconnectAsync(Interface interface)
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceForgetAsync(Interface interface, Network network)
{
    //TODO: add implementation
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
