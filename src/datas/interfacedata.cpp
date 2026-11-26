#include "interfacedata.h"

#include <mutex>

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
    m_opts = IfaceOption::IFACE_OPT_DEFAULT;
}

bool InterfaceData::operator==(const InterfaceData &other) const
{
    /*
     * Lock both datas at once: std::scoped_lock avoids deadlock
     * whatever the order used by concurrent comparisons
     */
    std::scoped_lock locker(m_mutex, other.m_mutex);

    /* Verify comparaison */
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
