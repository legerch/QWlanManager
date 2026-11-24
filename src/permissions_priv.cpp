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
    m_currentPerm = WlanPerm::WPERM_UNKNOWN;
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

/*!
 * \brief Update permission status
 * \details
 * Backends only report current status: storage and change
 * notification are performed here.
 *
 * \return
 * Returns current permission status.
 */
WlanPerm PermissionsPrivate::updateStatus()
{
    const WlanPerm idPerm = statusRetrieve();
    setStatus(idPerm);

    return idPerm;
}

void PermissionsPrivate::setStatus(WlanPerm idPerm)
{
    /* Update permission status */
    {
        QMutexLocker locker(&m_mutex);

        // Do status has changed ?
        if(idPerm == m_currentPerm){
            return;
        }

        m_currentPerm = idPerm;
    }

    /* Notify change (outside lock, a slot may call us back) */
    emit q_ptr->sPermissionsChanged(idPerm);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
