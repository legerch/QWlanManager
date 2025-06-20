#include "qwlanmanager/manager.h"

#include "factorybackend.h"
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

/*****************************/
/* Functions implementation  */
/*      Public Class         */
/*****************************/

Manager::Manager(QObject *parent) :
    QObject{parent},
    d_ptr(FactoryBackend::createBackend(this))
{
    d_ptr->initialize();
    d_ptr->interfaceListRefresh();
}

Manager::~Manager()
{
    d_ptr->terminate();
}

void Manager::doScan(const QUuid &idInterface)
{
    /* Retrieve associated interfaces */
    Interface iface = getInterface(idInterface);
    if(iface.isNull()){
        qWarning("Unable to perform scan, unknown interface ID [uuid: %s]", qUtf8Printable(idInterface.toString()));
        emit sScanFailed(idInterface, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Start scan request */
    d_ptr->interfaceScanNetworks(iface);
}

ListInterfaces Manager::getInterfaces() const
{
    return d_ptr->m_interfaces.values();
}

Interface Manager::getInterface(const QUuid &idInterface) const
{
    return d_ptr->m_interfaces.value(idInterface, nullptr);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
