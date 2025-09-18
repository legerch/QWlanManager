#include "enginenetworkmanager.h"

/*****************************/
/* Macro definitions         */
/*****************************/

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

EngineNetworkManager::EngineNetworkManager(Manager *parent)
    : ManagerPrivate(parent)
{
    /* Nothing to do */
}

EngineNetworkManager::~EngineNetworkManager()
{
    /* Nothing to do */
}

void EngineNetworkManager::initialize()
{
    //TODO: add implementation
}

void EngineNetworkManager::terminate()
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceListRefresh()
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceScanNetworksAsync(Interface interface)
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceConnectAsync(Interface interface, Network network, const QString &password)
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceDisconnectAsync(Interface interface)
{
    //TODO: add implementation
}

void EngineNetworkManager::interfaceForgetAsync(Interface interface, Network network)
{
    //TODO: add implementation
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
