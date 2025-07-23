#ifndef QWLANMAN_BACKEND_MOCK_PERMISSIONSMOCK_H
#define QWLANMAN_BACKEND_MOCK_PERMISSIONSMOCK_H

#include "permissions_priv.h"

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class PermissionsMock final : public PermissionsPrivate
{
public:
    explicit PermissionsMock(Permissions *parent);
    ~PermissionsMock();

public:
    void initialize() override;
    void terminate() override;

public:
    WlanPerm updateStatus() override;
    WlanError prompt() override;
    WlanError openParams() override;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_MOCK_PERMISSIONSMOCK_H
