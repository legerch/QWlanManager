#include "cachepolicy_priv.h"

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

CachePolicyPrivate::CachePolicyPrivate()
{
    m_maxScans = 0;
    m_maxDelay = 0;
}

bool CachePolicyPrivate::operator==(const CachePolicyPrivate &other) const
{
    return m_maxScans == other.m_maxScans
        && m_maxDelay == other.m_maxDelay;
}

bool CachePolicyPrivate::operator!=(const CachePolicyPrivate &other) const
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
