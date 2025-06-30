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

//Done : case 1 -> Network not known, good password
//Done : case 2 -> Network not known, bad password (really out)
//Done : case 3 -> Network not known, bad password (one letter wrong)
//Done : case 4 -> Network known and valid profile
//TODO : case 5 -> Network known and invalid profile (passkey wrong and network not forgotten)
//Done : case 5 -> Try to connect to already connected network
//Done : case 6 -> Try to connect to UTF-8 ssid
//Done : case 7 -> Try to connect to UTF-8 passkey
void Manager::doConnect(const QUuid &idInterface, const QString &ssid, const QString &password)
{
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
