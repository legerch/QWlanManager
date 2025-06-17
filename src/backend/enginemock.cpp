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

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
