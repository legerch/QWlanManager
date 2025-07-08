#include "qwlanmanager/datas/interface.h"

#include "interfacedata.h"

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
/*      Public Class         */
/*****************************/

Interface::Interface()
    : d_ptr(QSharedPointer<InterfaceData>::create())
{
    /* Nothing to do */
}

Interface::Interface(const Interface &other) = default;
Interface::Interface(Interface &&other) = default;

Interface::~Interface() = default;

bool Interface::isValid() const
{
    if(d_ptr.isNull()){
        return false;
    }

    return !getUid().isNull();
}

bool Interface::isBusy() const
{
    return getState() != IfaceState::IFACE_STS_IDLE;
}

IfaceState Interface::getState() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_state;
}

const QUuid& Interface::getUid() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_uid;
}

const QString& Interface::getHwAddress() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_hwAddress;
}

const QString& Interface::getName() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_name;
}

const QString& Interface::getDescription() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_description;
}

const CachePolicy &Interface::getCachePolicy() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_cachePolicy;
}

const MapNetworks& Interface::getMapNetworks() const
{
    return d_ptr->m_mapNets;
}

ListNetworks Interface::getListNetworks() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_mapNets.values();
}

Network Interface::getNetwork(const QString &ssid) const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_mapNets.value(ssid);
}

Network Interface::getNetworkConnected() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_mapNets.value(d_ptr->m_connectedSsid);
}

Interface& Interface::operator=(const Interface &other) = default;
Interface& Interface::operator=(Interface &&other) noexcept = default;

bool Interface::operator==(const Interface &other) const
{
    /* Verify if same pointer */
    if(d_ptr.data() == other.d_ptr.data()){
        return true;
    }

    /* One of the two is null, can't compare */
    if(!d_ptr || !other.d_ptr){
        return false;
    }

    /* Compare content */
    return *d_ptr == *other.d_ptr;
}

bool Interface::operator!=(const Interface &other) const
{
    return !(*this == other);
}

/*****************************/
/* Qt specific methods       */
/*****************************/

QDebug operator<<(QDebug debug, const Interface &interface)
{
    QDebugStateSaver saver(debug);
    if(!interface.isValid()){
        debug.nospace() << "Interface("
                        << "is valid: false)";

        return debug;
    }

    debug.nospace() << "Interface("
                    << "state: " << interface.getState() << ", "
                    << "uid: " << interface.getUid() << ", "
                    << "hw address: " << interface.getHwAddress() << ", "
                    << "name: " << interface.getName() << ", "
                    << "description: " << interface.getDescription() << ")";
    return debug;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
