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

const QString& NetworkData::getBssid() const
{
    return m_bssid;
}

const QString& NetworkData::getSsid() const
{
    return m_ssid;
}

const QString& NetworkData::getProfileName() const
{
    return m_profileName;
}

bool NetworkData::operator==(const NetworkData &other) const
{
    return m_bssid == other.m_bssid;
}

bool NetworkData::operator!=(const NetworkData &other) const
{
    return !(*this == other);
}

bool NetworkData::operator<(const NetworkData &other) const
{
    return m_bssid < other.m_bssid;
}

bool NetworkData::operator>(const NetworkData &other) const
{
    return m_bssid > other.m_bssid;
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
    return qHash(key.getBssid(), seed);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
