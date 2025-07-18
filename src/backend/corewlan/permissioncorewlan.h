#ifndef QWLANMAN_BACKEND_COREWLAN_PERMISSIONCOREWLAN_H
#define QWLANMAN_BACKEND_COREWLAN_PERMISSIONCOREWLAN_H

#include "permissions_priv.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class PermissionCoreWlan final : public PermissionsPrivate
{

public:
    explicit PermissionCoreWlan(Permissions *parent);
    ~PermissionCoreWlan();

public:
    void initialize() override;
    void terminate() override;

public:
    WlanPerm wlanRetrieve() override;
    bool wlanOpenParams() override;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_COREWLAN_PERMISSIONCOREWLAN_H
