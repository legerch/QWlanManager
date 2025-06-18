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

void IfaceMutator::setName(const QString &name)
{
    m_iface->m_name = name;
}

void IfaceMutator::setFriendlyName(const QString &fname)
{
    m_iface->m_friendlyName = fname;
}

void IfaceMutator::setDescription(const QString &desc)
{
    m_iface->m_description = desc;
}

void IfaceMutator::setIsUp(bool isUp)
{
    m_iface->m_isUp = isUp;
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
