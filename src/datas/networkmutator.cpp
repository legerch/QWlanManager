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

CacheInfo& NetworkMutator::getCacheRef()
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_cache;
}

void NetworkMutator::setDataEngine(const std::any &data)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_dataEngine = data;
}

const std::any& NetworkMutator::getDataEngine() const
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
