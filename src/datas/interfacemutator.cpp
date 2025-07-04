#include "interfacemutator.h"

#include "interfacedata.h"
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

InterfaceMutator::InterfaceMutator(const Interface &iface)
    : d_ptr(iface.d_ptr)
{
    /* Nothing to do */
}

void InterfaceMutator::setState(IfaceState idState)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_state = idState;
}

void InterfaceMutator::setUid(const QUuid &uid)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_uid = uid;
}

void InterfaceMutator::setHwAddress(const QString &hwAddr)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_hwAddress = hwAddr;
}

void InterfaceMutator::setHwAddress(const uchar *buffer, int lenBuffer)
{
    QString hwAddress;

    /* Format HW address */
    for(int i = 0; i < lenBuffer; ++i){
        if(i){
            hwAddress.append(":");
        }

        const QString strHex = QString("%1").arg(buffer[i], 2, 16, QChar('0'));
        hwAddress.append(strHex.toUpper());
    }

    /* Set HW address */
    setHwAddress(hwAddress);
}

void InterfaceMutator::setName(const QString &name)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_name = name;
}

void InterfaceMutator::setDescription(const QString &desc)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_description = desc;
}

void InterfaceMutator::setConnectedSsid(const QString &ssid)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_connectedSsid = ssid;
}

MapNetworks& InterfaceMutator::getMapNetworksRef()
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_mapNets;
}

void InterfaceMutator::setCachePolicy(const CachePolicy &cachePolicy)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_cachePolicy = cachePolicy;
}

CachePolicy& InterfaceMutator::getCachePolicyRef()
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_cachePolicy;
}

void InterfaceMutator::setDataEngine(const std::any &data)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_dataEngine = data;
}

const std::any& InterfaceMutator::getDataEngine() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_dataEngine;
}

void InterfaceMutator::updateNetworksCached(const MapNetworks &oldNets, const QDateTime &now)
{
    for(auto it = oldNets.cbegin(); it != oldNets.cend(); ++it){
        Network net = it.value();
        NetworkMutator munet(net);

        CacheInfo &cache = munet.getCacheRef();

        // Update cache
        cache.markUnseen();

        // Do cache is expired ?
        if(cache.isExpired(d_ptr->m_cachePolicy, now)){
            continue;
        }

        // Cache not expired, keep the network
        d_ptr->m_mapNets.insert(net.getSsid(), net);
    }
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
