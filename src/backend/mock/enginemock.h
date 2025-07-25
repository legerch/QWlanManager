#ifndef QWLANMAN_BACKEND_MOCK_ENGINEMOCK_H
#define QWLANMAN_BACKEND_MOCK_ENGINEMOCK_H

#include "manager_priv.h"

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class EngineMock final : public ManagerPrivate
{
public:
    explicit EngineMock(Manager *parent);
    ~EngineMock();

public:
    void initialize() override;
    void terminate() override;

public:
    void interfaceListRefresh() override;

protected:
    void interfaceScanNetworksAsync(Interface interface) override;
    void interfaceConnectAsync(Interface interface, Network network, const QString &password) override;
    void interfaceDisconnectAsync(Interface interface) override;
    void interfaceForgetAsync(Interface interface, Network network) override;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_MOCK_ENGINEMOCK_H
