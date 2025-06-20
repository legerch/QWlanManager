#include "qwlanmanager/interface.h"

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

InterfaceData::InterfaceData()
{
    m_state = IfaceState::IFACE_STS_IDLE;
}

IfaceState InterfaceData::getState() const
{
    QMutexLocker locker(&m_mutex);
    return m_state;
}

const QUuid& InterfaceData::getUid() const
{
    QMutexLocker locker(&m_mutex);
    return m_uid;
}

const QString& InterfaceData::getHwAddress() const
{
    QMutexLocker locker(&m_mutex);
    return m_hwAddress;
}

const QString& InterfaceData::getName() const
{
    QMutexLocker locker(&m_mutex);
    return m_name;
}

const QString& InterfaceData::getDescription() const
{
    QMutexLocker locker(&m_mutex);
    return m_description;
}

const MapNetworks& InterfaceData::getMapNetworks() const
{
    return m_mapNets;
}

ListNetworks InterfaceData::getListNetworks() const
{
    QMutexLocker locker(&m_mutex);
    return m_mapNets.values();
}

Network InterfaceData::getNetwork(const QString &ssid) const
{
    QMutexLocker locker(&m_mutex);
    return m_mapNets.value(ssid, nullptr);
}

Network InterfaceData::getNetworkConnected() const
{
    QMutexLocker locker(&m_mutex);
    return m_mapNets.value(m_connectedSsid, nullptr);
}

bool InterfaceData::operator==(const InterfaceData &other) const
{
    return m_uid == other.m_uid;
}

bool InterfaceData::operator!=(const InterfaceData &other) const
{
    return !(*this == other);
}

bool InterfaceData::operator<(const InterfaceData &other) const
{
    return m_uid < other.m_uid;
}

bool InterfaceData::operator>(const InterfaceData &other) const
{
    return m_uid > other.m_uid;
}

bool InterfaceData::operator<=(const InterfaceData &other) const
{
    return !(*this > other);
}

bool InterfaceData::operator>=(const InterfaceData &other) const
{
    return !(*this < other);
}

/*****************************/
/* Qt custom related methods */
/*****************************/

size_t qHash(const InterfaceData &key, uint seed)
{
    return qHash(key.getUid(), seed);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
