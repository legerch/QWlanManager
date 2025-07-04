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
    /* Nothing to do */
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
