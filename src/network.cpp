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

Network::Network() = default;

const QString& Network::getBssid() const
{
    return m_bssid;
}

const QString& Network::getSsid() const
{
    return m_ssid;
}

const QString& Network::getProfileName() const
{
    return m_profileName;
}

bool Network::operator==(const Network &other) const
{
    return m_bssid == other.m_bssid;
}

bool Network::operator!=(const Network &other) const
{
    return !(*this == other);
}

bool Network::operator<(const Network &other) const
{
    return m_bssid < other.m_bssid;
}

bool Network::operator>(const Network &other) const
{
    return m_bssid > other.m_bssid;
}

bool Network::operator<=(const Network &other) const
{
    return !(*this > other);
}

bool Network::operator>=(const Network &other) const
{
    return !(*this < other);
}

/*****************************/
/* Qt custom related methods */
/*****************************/

size_t qHash(const Network &key, uint seed)
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
