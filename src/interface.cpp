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

InterfaceData::InterfaceData() = default;

const QUuid& InterfaceData::getUid() const
{
    return m_uid;
}

const QString& InterfaceData::getHwAddress() const
{
    return m_hwAddress;
}

const QString& InterfaceData::getName() const
{
    return m_name;
}

const QString& InterfaceData::getFriendlyName() const
{
    return m_friendlyName;
}

const QString& InterfaceData::getDescription() const
{
    return m_description;
}

bool InterfaceData::isUp() const
{
    return m_isUp;
}

const MapNetworks& InterfaceData::getMapNetworks() const
{
    return m_mapNets;
}

ListNetworks InterfaceData::getListNetworks() const
{
    return m_mapNets.values();
}

Network InterfaceData::getNetwork(const QString &ssid) const
{
    return m_mapNets.value(ssid, nullptr);
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
