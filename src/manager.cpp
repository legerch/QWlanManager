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
    /* Emit start signal */
    emit sScanStarted(idInterface);

    /* Retrieve associated interface */
    Interface iface = getInterface(idInterface);
    if(!iface.isValid()){
        qWarning("Unable to perform scan, unknown interface ID [uuid: %s]", qUtf8Printable(idInterface.toString()));
        emit sScanFailed(idInterface, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Start scan request */
    d_ptr->interfaceScanNetworks(iface);
}

void Manager::doConnect(const QUuid &idInterface, const QString &ssid, const QString &password)
{
    /* Emit start signal */
    emit sConnectionStarted(idInterface, ssid);

    /* Retrieve associated interface */
    Interface iface = getInterface(idInterface);
    if(!iface.isValid()){
        qWarning("Unable to perform connection, unknown interface ID [uuid: %s]", qUtf8Printable(idInterface.toString()));
        emit sConnectionFailed(idInterface, ssid, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Retrieve associated network */
    Network net = iface.getNetwork(ssid);
    if(!net.isValid()){
        qWarning("Unable to perform connection, unknown network SSID [ssid: %s]", qUtf8Printable(ssid));
        emit sConnectionFailed(idInterface, ssid, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Start connection request */
    d_ptr->interfaceConnect(iface, net, password);
}

void Manager::doDisconnect(const QUuid &idInterface)
{
    /* Emit start signal */
    emit sDisconnectionStarted(idInterface);

    /* Retrieve associated interface */
    Interface iface = getInterface(idInterface);
    if(!iface.isValid()){
        qWarning("Unable to perform disconnection, unknown interface ID [uuid: %s]", qUtf8Printable(idInterface.toString()));
        emit sDisconnectionFailed(idInterface, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Start disconnection request */
    d_ptr->interfaceDisconnect(iface);
}

void Manager::doForget(const QUuid &idInterface, const QString &ssid)
{
    /* Emit start signal */
    emit sForgetStarted(idInterface, ssid);

    /* Retrieve associated interface */
    Interface iface = getInterface(idInterface);
    if(!iface.isValid()){
        qWarning("Unable to forget network, unknown interface ID [uuid: %s]", qUtf8Printable(idInterface.toString()));
        emit sForgetFailed(idInterface, ssid, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Retrieve associated network */
    Network net = iface.getNetwork(ssid);
    if(!net.isValid()){
        qWarning("Unable to forget network, unknown network SSID [ssid: %s]", qUtf8Printable(ssid));
        emit sForgetFailed(idInterface, ssid, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Start connection request */
    d_ptr->interfaceForget(iface, net);
}

ListInterfaces Manager::getInterfaces() const
{
    return d_ptr->m_interfaces.values();
}

Interface Manager::getInterface(const QUuid &idInterface) const
{
    return d_ptr->m_interfaces.value(idInterface);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
