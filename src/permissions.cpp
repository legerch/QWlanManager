#include "qwlanmanager/permissions.h"

#include "factorybackend.h"
#include "permissions_priv.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class qwm::Permissions
 *
 * \brief Use to manage all \b permissions related operations
 *
 * \sa qwm::Manager
 */

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

/*!
 * \brief Construct a wlan permission object
 * \details
 * Allow to perform needed backend initialization
 *
 * \param[out] parent
 * The parent argument is passed to QObject's constructor
 */
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

/*!
 * \brief Allow to retrieve current wlan permissions status
 *
 * \return
 * Returns permissions status.
 *
 * \sa wlanOpenParams()
 */
WlanPerm Permissions::wlanRetrieve()
{
    return d_ptr->wlanRetrieve();
}

/*!
 * \brief Allow to open associated OS parameters
 * \details
 * Permissions cannot be set via native API, it's the user
 * responsibility to give your application required permissions. \n
 * But we can ease the process by opening the related parameter
 * window.
 *
 * \return
 * Returns \c true if succeed to open permission parameter window.
 *
 * \sa wlanRetrieve()
 */
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
