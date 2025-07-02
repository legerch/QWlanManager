#include "qwlanmanager/permissions.h"

#include "factorybackend.h"
#include "permissions_priv.h"

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

Permissions::Permissions(QObject *parent) :
    QObject{parent},
    d_ptr(FactoryBackend::createPermissions(this))
{
    d_ptr->initialize();
}

Permissions::~Permissions()
{
    d_ptr->terminate();
}

WlanPerm Permissions::wlanRetrieve()
{
    return d_ptr->wlanRetrieve();
}

bool Permissions::wlanOpenParams()
{
    return d_ptr->wlanOpenParams();
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
