#include "qwlanmanager/network.h"

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

NetworkData::NetworkData() = default;

const QString& NetworkData::getSsid() const
{
    QMutexLocker locker(&m_mutex);
    return m_ssid;
}

const QString& NetworkData::getProfileName() const
{
    QMutexLocker locker(&m_mutex);
    return m_profileName;
}

bool NetworkData::operator==(const NetworkData &other) const
{
    return m_ssid == other.m_ssid
        && m_profileName == other.m_profileName;
}

bool NetworkData::operator!=(const NetworkData &other) const
{
    return !(*this == other);
}

bool NetworkData::operator<(const NetworkData &other) const
{
    if(m_ssid != other.m_ssid){
        return m_ssid < other.m_ssid;
    }

    return m_profileName < other.m_profileName;
}

bool NetworkData::operator>(const NetworkData &other) const
{
    if(m_ssid != other.m_ssid){
        return m_ssid > other.m_ssid;
    }

    return m_profileName > other.m_profileName;
}

bool NetworkData::operator<=(const NetworkData &other) const
{
    return !(*this > other);
}

bool NetworkData::operator>=(const NetworkData &other) const
{
    return !(*this < other);
}

/*****************************/
/* Qt custom related methods */
/*****************************/

size_t qHash(const NetworkData &key, uint seed)
{
    return qHashMulti(seed, key.getSsid(), key.getProfileName());
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
