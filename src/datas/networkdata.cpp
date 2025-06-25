#include "networkdata.h"

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
/*****************************/

NetworkData::NetworkData()
{
    m_idAuth = AuthAlgo::AUTH_ALGO_UNKNOWN;
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

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
