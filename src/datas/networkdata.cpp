#include "networkdata.h"

#include <mutex>

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
    m_idCipher = CipherAlgo::CIPHER_ALGO_UNKNOWN;
    m_signalQuality = 0;
}

bool NetworkData::operator==(const NetworkData &other) const
{
    /*
     * Lock both datas at once: std::scoped_lock avoids deadlock
     * whatever the order used by concurrent comparisons
     */
    std::scoped_lock locker(m_mutex, other.m_mutex);

    /* Verify comparaison */
    return m_ssid == other.m_ssid
        && m_profileName == other.m_profileName
        && m_idAuth == other.m_idAuth
        && m_idCipher == other.m_idCipher;
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
