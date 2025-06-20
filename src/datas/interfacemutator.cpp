#include "interfacemutator.h"

#include "interfacedata.h"

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
    : m_iface(iface)
{
    /* Nothing to do */
}

void InterfaceMutator::setState(IfaceState idState)
{
    QMutexLocker locker(&m_iface.d_ptr->m_mutex);
    m_iface.d_ptr->m_state = idState;
}

void InterfaceMutator::setUid(const QUuid &uid)
{
    QMutexLocker locker(&m_iface.d_ptr->m_mutex);
    m_iface.d_ptr->m_uid = uid;
}

void InterfaceMutator::setHwAddress(const QString &hwAddr)
{
    QMutexLocker locker(&m_iface.d_ptr->m_mutex);
    m_iface.d_ptr->m_hwAddress = hwAddr;
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
    QMutexLocker locker(&m_iface.d_ptr->m_mutex);
    m_iface.d_ptr->m_name = name;
}

void InterfaceMutator::setDescription(const QString &desc)
{
    QMutexLocker locker(&m_iface.d_ptr->m_mutex);
    m_iface.d_ptr->m_description = desc;
}

void InterfaceMutator::setConnectedSsid(const QString &ssid)
{
    QMutexLocker locker(&m_iface.d_ptr->m_mutex);
    m_iface.d_ptr->m_connectedSsid = ssid;
}

MapNetworks& InterfaceMutator::getMapNetworksRef()
{
    QMutexLocker locker(&m_iface.d_ptr->m_mutex);
    return m_iface.d_ptr->m_mapNets;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
