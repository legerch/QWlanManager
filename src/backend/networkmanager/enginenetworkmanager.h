#ifndef QWLANMAN_BACKEND_NM_ENGINENETWORKMANAGER_H
#define QWLANMAN_BACKEND_NM_ENGINENETWORKMANAGER_H

#include "manager_priv.h"

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class EngineNetworkManager final : public ManagerPrivate
{
public:
    explicit EngineNetworkManager(Manager *parent);
    ~EngineNetworkManager();

public:
    void initialize() override;
    void terminate() override;

protected:
    void interfaceListRefresh() override;
    void interfaceScanNetworksAsync(Interface interface) override;
    void interfaceConnectAsync(Interface interface, Network network, const QString &password) override;
    void interfaceDisconnectAsync(Interface interface) override;
    void interfaceForgetAsync(Interface interface, Network network) override;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_NM_ENGINENETWORKMANAGER_H
