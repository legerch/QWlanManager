#ifndef QWLANMAN_PERMISSIONS_PRIV_H
#define QWLANMAN_PERMISSIONS_PRIV_H

#include "qwlanmanager/permissions.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

//TODO: don't duplicate "m_currentPerm" attribution in each backend
class PermissionsPrivate
{
    QWLANMAN_DISABLE_COPY(PermissionsPrivate)
    Q_DECLARE_PUBLIC(Permissions)

public:
    explicit PermissionsPrivate(Permissions *parent);

    virtual ~PermissionsPrivate();

public:
    virtual void initialize() = 0;
    virtual void terminate() = 0;

public:
    virtual WlanPerm wlanRetrieve() = 0;
    virtual bool wlanOpenParams() = 0;

protected:
    WlanPerm m_currentPerm;

    Permissions *q_ptr;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_PERMISSIONS_PRIV_H
