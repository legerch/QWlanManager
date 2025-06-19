#include "ifacemutator.h"

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

IfaceMutator::IfaceMutator(const Interface &iface)
    : m_iface(iface)
{
    /* Nothing to do */
}

void IfaceMutator::setUid(const QUuid &uid)
{
    m_iface->m_uid = uid;
}

void IfaceMutator::setHwAddress(const QString &hwAddr)
{
    m_iface->m_hwAddress = hwAddr;
}

void IfaceMutator::setHwAddress(const uchar *buffer, int lenBuffer)
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

void IfaceMutator::setName(const QString &name)
{
    m_iface->m_name = name;
}

void IfaceMutator::setDescription(const QString &desc)
{
    m_iface->m_description = desc;
}

MapNetworks& IfaceMutator::getMapNetworksRef()
{
    return m_iface->m_mapNets;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
