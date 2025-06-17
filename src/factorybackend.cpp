#include "factorybackend.h"

#include "backend/enginemock.h"

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

std::unique_ptr<ManagerPrivate> FactoryBackend::createBackend(Manager *parent)
{
    return std::make_unique<EngineMock>(parent);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
