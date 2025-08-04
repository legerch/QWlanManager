#include "enginemock.h"

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

EngineMock::EngineMock(Manager *parent)
    : ManagerPrivate(parent)
{
    qDebug("Engine mock constructor called");
}

EngineMock::~EngineMock()
{
    qDebug("Engine mock destructor called");
}

void EngineMock::initialize()
{
    qDebug("Engine mock inititialization called");
}

void EngineMock::terminate()
{
    qDebug("Engine mock termination called");
}

void EngineMock::interfaceListRefresh()
{
    /* Reset list of interfaces */
    m_currentIfaces.clear();

    /* Fill with mock values */
    Interface iface1;
    InterfaceMutator miface1(iface1);
    miface1.setUid(QUuid::createUuid());
    miface1.setHwAddress("11:22:33:44:55:66");
    miface1.setName("Wifi 1");
    miface1.setDescription("Example of adapter 1");

    Interface iface2;
    InterfaceMutator miface2(iface2);
    miface2.setUid(QUuid::createUuid());
    miface2.setHwAddress("77:88:99:00:AA:BB");
    miface2.setName("Wifi 2");
    miface2.setDescription("Example of adapter 2");

    /* Add interface to map */
    m_currentIfaces.insert(iface1.getUid(), iface1);
    m_currentIfaces.insert(iface2.getUid(), iface2);
}

void EngineMock::interfaceScanNetworksAsync(Interface interface)
{
    handleScanDone(interface, WlanError::WERR_OPERATION_UNSUPPORTED);
}

void EngineMock::interfaceConnectAsync(Interface interface, Network network, QWLANMAN_VAR_UNUSED const QString &password)
{
    handleConnectDone(interface, network.getSsid(), WlanError::WERR_OPERATION_UNSUPPORTED);
}

void EngineMock::interfaceDisconnectAsync(Interface interface)
{
    handleDisconnectDone(interface, WlanError::WERR_OPERATION_UNSUPPORTED);
}

void EngineMock::interfaceForgetAsync(Interface interface, Network network)
{
    handleForgetDone(interface, network, WlanError::WERR_OPERATION_UNSUPPORTED);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
