#include "networkmutator.h"

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
    QMutexLocker locker(&m_network->m_mutex);
    m_network->m_ssid = ssid;
}

void NetworkMutator::setProfileName(const QString &profile)
{
    QMutexLocker locker(&m_network->m_mutex);
    m_network->m_profileName = profile;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
