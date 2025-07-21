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

//TODO: don't duplicate "m_currentPerm" attribution in each backend
//TODO: do a first initialization at construction
class PermissionsPrivate
{
    QWLANMAN_DISABLE_COPY(PermissionsPrivate)
    Q_DECLARE_PUBLIC(Permissions)

public:
    explicit PermissionsPrivate(Permissions *parent);

    virtual ~PermissionsPrivate();

public:
    WlanPerm getStatus() const;
    void setStatus(WlanPerm idPerm);

public:
    virtual void initialize() = 0;
    virtual void terminate() = 0;

public:
    virtual WlanPerm updateStatus() = 0;
    virtual WlanError prompt() = 0;
    virtual WlanError openParams() = 0;

protected:
    mutable QMutex m_mutex;
    WlanPerm m_currentPerm;

    Permissions *q_ptr;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_PERMISSIONS_PRIV_H
