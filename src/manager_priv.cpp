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
    if(interface->getState() != IfaceState::IFACE_STS_IDLE){
        qWarning("Unable to perform scan, interface is busy [uuid: %s, state: %s (%d)]",
                 qUtf8Printable(interface->getUid().toString()),
                 qUtf8Printable(ifaceStateToString(interface->getState())),
                 interface->getState()
        );

        emit q_ptr->sScanFailed(interface->getUid(), WlanError::WERR_IFACE_BUSY);
        return;
    }

    /* Start scan request */
    interfaceScanNetworksAsync(interface);
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
