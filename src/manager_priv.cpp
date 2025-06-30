#include "manager_priv.h"

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

ManagerPrivate::ManagerPrivate(Manager *parent)
    : q_ptr(parent)
{
    /* Nothing to do */
}

ManagerPrivate::~ManagerPrivate()
{
    /* Nothing to do */
}

void ManagerPrivate::interfaceScanNetworks(Interface interface)
{
    /* Verify that interface can perform a scan */
    if(interface.getState() != IfaceState::IFACE_STS_IDLE){
        qWarning("Unable to perform scan, interface is busy [uuid: %s, state: %s (%d)]",
                 qUtf8Printable(interface.getUid().toString()),
                 qUtf8Printable(ifaceStateToString(interface.getState())),
                 interface.getState()
        );

        emit q_ptr->sScanFailed(interface.getUid(), WlanError::WERR_IFACE_BUSY);
        return;
    }

    /* Start scan request */
    interfaceScanNetworksAsync(interface);
}

//TODO: enqueue connection request (because, performing connection during scan is undefined behaviour but hard to be out this window !)
void ManagerPrivate::interfaceConnect(Interface interface, Network network, const QString &password)
{
    /* Verify that interface is not already connected */
    const Network current = interface.getNetworkConnected();
    if(interface.getNetworkConnected() == network){
        qDebug("Interface '%s' is already connected to '%s', nothing to perform", qUtf8Printable(interface.getName()), qUtf8Printable(network.getSsid()));

        emit q_ptr->sConnectionSucceed(interface.getUid(), network.getSsid());
        return;
    }

    /* Verify that interface can perform connection */
    if(interface.getState() != IfaceState::IFACE_STS_IDLE){
        qWarning("Unable to perform connection, interface is busy [uuid: %s, state: %s (%d)]",
                 qUtf8Printable(interface.getUid().toString()),
                 qUtf8Printable(ifaceStateToString(interface.getState())),
                 interface.getState()
        );

        emit q_ptr->sConnectionFailed(interface.getUid(), network.getSsid(), WlanError::WERR_IFACE_BUSY);
        return;
    }

    /* Start connection request */
    interfaceConnectAsync(interface, network, password);
}

void ManagerPrivate::interfaceDisconnect(Interface interface)
{
    /* Verify that interface is not already disconnected */
    const Network current = interface.getNetworkConnected();
    if(!current.isValid()){
        qDebug("Interface '%s' is already disconnected, nothing to perform", qUtf8Printable(interface.getName()));

        emit q_ptr->sDisconnectionSucceed(interface.getUid());
        return;
    }

    /* Verify that interface can perform disconnection */
    if(interface.getState() != IfaceState::IFACE_STS_IDLE){
        qWarning("Unable to perform disconnection, interface is busy [uuid: %s, state: %s (%d)]",
                 qUtf8Printable(interface.getUid().toString()),
                 qUtf8Printable(ifaceStateToString(interface.getState())),
                 interface.getState()
        );

        emit q_ptr->sDisconnectionFailed(interface.getUid(), WlanError::WERR_IFACE_BUSY);
        return;
    }

    /* Start disconnection request */
    interfaceDisconnectAsync(interface);
}

/*****************************/
/* Functions implementation  */
/*****************************/

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
