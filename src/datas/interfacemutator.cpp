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

/*!
 * \brief Set interface networks and connected network
 *
 * \param[in] mapNets
 * Networks of the interface.
 * \param[in] connectedSsid
 * SSID of the network currently connected, empty if none.
 */
void InterfaceMutator::setNetworks(const MapNetworks &mapNets, const QString &connectedSsid)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_mapNets = mapNets;
    d_ptr->m_connectedSsid = connectedSsid;
}

/*!
 * \brief Retrieve requests queue of the interface
 *
 * \warning
 * Returned reference is not protected by the interface lock: requests queue
 * is an internal data, it must only be used from the manager thread.
 *
 * \return
 * Returns reference to the interface requests queue.
 */
RequestsQueue& InterfaceMutator::getRequestsQueueRef()
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_reqsQueue;
}

/*!
 * \brief Set engine data related to the interface
 *
 * \param[in] data
 * Engine data to store. \n
 * Only store handles (shared pointers, pointers, etc...) since this data
 * is copied each time it is retrieved.
 *
 * \sa getDataEngine()
 */
void InterfaceMutator::setDataEngine(const std::any &data)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_dataEngine = data;
}

/*!
 * \brief Retrieve engine data related to the interface
 *
 * \return
 * Returns a copy of the stored engine data (handles are copied,
 * not the objects they refer to).
 *
 * \sa setDataEngine()
 */
std::any InterfaceMutator::getDataEngine() const
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
