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
    : m_network(network)
{
    /* Nothing to do */
}

void NetworkMutator::setSsid(const QString &ssid)
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    m_network.d_ptr->m_ssid = ssid;
}

void NetworkMutator::setProfileName(const QString &profile)
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    m_network.d_ptr->m_profileName = profile;
}

void NetworkMutator::setAuthAlgo(AuthAlgo idAuth)
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    m_network.d_ptr->m_idAuth = idAuth;
}

void NetworkMutator::setCipherAlgo(CipherAlgo idCipher)
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    m_network.d_ptr->m_idCipher = idCipher;
}

void NetworkMutator::setSignalQuality(uint quality)
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    m_network.d_ptr->m_signalQuality = quality;
}

CacheInfo& NetworkMutator::getCacheRef()
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    return m_network.d_ptr->m_cache;
}

void NetworkMutator::setDataEngine(const std::any &data)
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    m_network.d_ptr->m_dataEngine = data;
}

const std::any& NetworkMutator::getDataEngine() const
{
    QMutexLocker locker(&m_network.d_ptr->m_mutex);
    return m_network.d_ptr->m_dataEngine;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
