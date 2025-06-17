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

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
