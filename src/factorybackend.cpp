#include "factorybackend.h"

#include <QtSystemDetection>

#if USE_ENGINE_WINNATIVE
#   include "backend/winnative/enginewinnative.h"
#else
#   include "backend/mock/enginemock.h"
#endif

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
#if USE_ENGINE_WINNATIVE
    qDebug("Use engine \"Windows Native\"");
    return std::make_unique<EngineWinNative>(parent);
#else
    qDebug("Use engine \"Mock\"");
    return std::make_unique<EngineMock>(parent);
#endif
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
