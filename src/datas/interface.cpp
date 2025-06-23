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

bool Interface::isValid() const
{
    if(d_ptr.isNull()){
        return false;
    }

    return !getUid().isNull();
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
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
