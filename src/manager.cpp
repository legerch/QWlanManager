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
    d_ptr->refreshInterfaces();
}

Manager::~Manager()
{
    d_ptr->terminate();
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
