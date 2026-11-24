#include "networkmutator.h"

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

NetworkMutator::NetworkMutator(const Network &network)
    : d_ptr(network.d_ptr)
{
    /* Nothing to do */
}

void NetworkMutator::setSsid(const QString &ssid)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_ssid = ssid;
}

void NetworkMutator::setProfileName(const QString &profile)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_profileName = profile;
}

void NetworkMutator::setAuthAlgo(AuthAlgo idAuth)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_idAuth = idAuth;
}

void NetworkMutator::setCipherAlgo(CipherAlgo idCipher)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_idCipher = idCipher;
}

void NetworkMutator::setSignalQuality(uint quality)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_signalQuality = quality;
}

/*!
 * \brief Update network properties from a network
 *
 * \param[in] network
 * Network to copy (often from a scan), containing new properties.
 */
void NetworkMutator::updateFrom(const Network &network)
{
    /* Retrieve new properties */
    const QString profile = network.getProfileName();
    const AuthAlgo idAuth = network.getAuthAlgo();
    const CipherAlgo idCipher = network.getCipherAlgo();
    const uint quality = network.getSignalQuality();
    const std::any dataEngine = NetworkMutator(network).getDataEngine();

    /* Update properties */
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_profileName = profile;
    d_ptr->m_idAuth = idAuth;
    d_ptr->m_idCipher = idCipher;
    d_ptr->m_signalQuality = quality;
    d_ptr->m_dataEngine = dataEngine;
}

/*!
 * \brief Retrieve cache informations of the network
 *
 * \warning
 * Returned reference is not protected by the network lock: cache informations
 * are internal datas, they must only be used from the manager thread.
 *
 * \return
 * Returns reference to the network cache informations.
 */
CacheInfo& NetworkMutator::getCacheRef()
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_cache;
}

/*!
 * \brief Set engine data related to the network
 *
 * \param[in] data
 * Engine data to store. \n
 * Only store handles (shared pointers, pointers, etc...) since this data
 * is copied each time it is retrieved.
 *
 * \sa getDataEngine()
 */
void NetworkMutator::setDataEngine(const std::any &data)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_dataEngine = data;
}

/*!
 * \brief Retrieve engine data related to the network
 *
 * \return
 * Returns a copy of the stored engine data (handles are copied,
 * not the objects they refer to).
 *
 * \sa setDataEngine()
 */
std::any NetworkMutator::getDataEngine() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_dataEngine;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
