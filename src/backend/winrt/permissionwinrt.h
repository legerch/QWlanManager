#ifndef QWLANMAN_BACKEND_WINRT_PERMISSIONWINRT_H
#define QWLANMAN_BACKEND_WINRT_PERMISSIONWINRT_H

#include "permissions_priv.h"
#include "typeswinrt.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class PermissionWinRt final : public PermissionsPrivate
{

public:
    explicit PermissionWinRt(Permissions *parent);
    ~PermissionWinRt();

public:
    void initialize() override;
    void terminate() override;

public:
    WlanPerm updateStatus() override;
    WlanError prompt() override;
    WlanError openParams() override;

private:
    void eventsRegister();
    void eventsUnregister();

private:
    WinRt::PermissionApp m_permsWlan{nullptr};
    WinRt::EventToken m_tokenAccessChanged;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_WINRT_PERMISSIONWINRT_H
