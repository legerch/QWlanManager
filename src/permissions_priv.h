#ifndef QWLANMAN_PERMISSIONS_PRIV_H
#define QWLANMAN_PERMISSIONS_PRIV_H

#include "qwlanmanager/permissions.h"

#include <QMutex>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class PermissionsPrivate
{
    QWLANMAN_DISABLE_COPY(PermissionsPrivate)
    Q_DECLARE_PUBLIC(Permissions)

public:
    explicit PermissionsPrivate(Permissions *parent);

    virtual ~PermissionsPrivate();

public:
    WlanPerm getStatus() const;
    WlanPerm updateStatus();

public:
    virtual void initialize() = 0;
    virtual void terminate() = 0;

public:
    virtual WlanError prompt() = 0;
    virtual WlanError openParams() = 0;

protected:
    virtual WlanPerm statusRetrieve() = 0;

private:
    void setStatus(WlanPerm idPerm);

protected:
    Permissions *q_ptr;

private:
    mutable QMutex m_mutex;
    WlanPerm m_currentPerm;     /*!< Permission status, only managed by this class: backends report it via statusRetrieve() */
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_PERMISSIONS_PRIV_H
