#ifndef QWLANMAN_BACKEND_COREWLAN_PERMISSIONCOREWLAN_H
#define QWLANMAN_BACKEND_COREWLAN_PERMISSIONCOREWLAN_H

#include "permissions_priv.h"

#include <any>

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
    WlanPerm updateStatus() override;
    WlanError prompt() override;
    WlanError openParams() override;

private:
    std::any m_permsWlan{nullptr};  // Use "std::any" type since type needed is an objective-c++ type
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_COREWLAN_PERMISSIONCOREWLAN_H
