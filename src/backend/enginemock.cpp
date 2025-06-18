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

void EngineMock::refreshInterfaces()
{
    /* Reset list of interfaces */
    m_interfaces.clear();

    /* Fill with mock values */
    Interface iface1 = Interface::create();
    IfaceMutator miface1(iface1);
    miface1.setUid(QUuid::createUuid());
    miface1.setHwAddress("11:22:33:44:55:66");
    miface1.setName("adapter_brandid_1");
    miface1.setFriendlyName("Wifi 1");
    miface1.setDescription("Example of adapter 1");
    miface1.setIsUp(true);

    Interface iface2 = Interface::create();
    IfaceMutator miface2(iface2);
    miface2.setUid(QUuid::createUuid());
    miface2.setHwAddress("77:88:99:00:AA:BB");
    miface2.setName("adapter_brandid_2");
    miface2.setFriendlyName("Wifi 2");
    miface2.setDescription("Example of adapter 2");
    miface2.setIsUp(true);

    /* Add interface to map */
    m_interfaces.insert(iface1->getUid(), iface1);
    m_interfaces.insert(iface2->getUid(), iface2);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
