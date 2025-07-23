#include "permissions_priv.h"

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

PermissionsPrivate::PermissionsPrivate(Permissions *parent)
    : q_ptr(parent)
{
    /* Nothing to do */
}

PermissionsPrivate::~PermissionsPrivate()
{
    /* Nothing to do */
}

WlanPerm PermissionsPrivate::getStatus() const
{
    QMutexLocker locker(&m_mutex);
    return m_currentPerm;
}

void PermissionsPrivate::setStatus(WlanPerm idPerm)
{
    QMutexLocker locker(&m_mutex);

    /* Do status has changed ? */
    if(idPerm == m_currentPerm){
        return;
    }

    /* Update permission status */
    m_currentPerm = idPerm;
    emit q_ptr->sPermissionsChanged(m_currentPerm);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
