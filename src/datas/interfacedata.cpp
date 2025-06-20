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

InterfaceData::InterfaceData()
{
    m_state = IfaceState::IFACE_STS_IDLE;
}

bool InterfaceData::operator==(const InterfaceData &other) const
{
    return m_uid == other.m_uid;
}

bool InterfaceData::operator!=(const InterfaceData &other) const
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
