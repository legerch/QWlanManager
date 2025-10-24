#include "datasdelegates.h"

#include <QDBusConnection>

/*****************************/
/* Class documentations      */
/*****************************/

//TODO: doc: qdbus --system org.freedesktop.NetworkManager /org/freedesktop/NetworkManager/Devices/1 org.freedesktop.DBus.Introspectable.Introspect
// https://networkmanager.dev/docs/api/latest/gdbus-org.freedesktop.NetworkManager.Device.Wireless.html

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/
namespace qwm
{

/*****************************/
/* Constants definitions     */
/*****************************/

/*****************************/
/* Functions implementation  */
/*    DelegateInterface      */
/*****************************/

DelegateInterface::DelegateInterface(Interface *parent, const QString &devicePath)
    : QObject(), m_refParent(parent), m_devPath(devicePath)
{
    eventRegister();
}

DelegateInterface::~DelegateInterface()
{
    eventUnregister();
}

void DelegateInterface::handlePropertiesChanged(const QString &arg1, const QVariantMap &propsChanged, const QStringList &propsInvalidated)
{
    //TODO: implement
}

void DelegateInterface::eventRegister()
{
    QDBusConnection *apiBus = QDBusConnection::systemBus();
}

void DelegateInterface::eventUnregister()
{

}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
