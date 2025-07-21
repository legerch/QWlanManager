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

/*!
 * \fn Permissions::sPermissionsChanged()
 * \brief Emitted when permissions status changes.
 *
 * \param[in] idPerm
 * New permission status.
 *
 * \sa retrieveWlanPermissions(), askWlanPermissions()
 */

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
    d_ptr->updateStatus();
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
 * \sa askWlanPermissions()
 */
WlanPerm Permissions::retrieveWlanPermissions()
{
    return d_ptr->updateStatus();
}

/*!
 * \brief Allow to ask for needed permissions
 * \details
 * Permissions cannot be set via native API, it's the user
 * responsibility to give your application required permissions. \n
 * So this method will ease the process by opening the related parameter
 * window.
 *
 * \return
 * Returns \c WlanError::WERR_NO_ERROR if succeed,
 * otherwise see \c WlanError for more details.
 *
 * \sa retrieveWlanPermissions()
 * \sa sPermissionsChanged()
 */
WlanError Permissions::askWlanPermissions()
{
    /* Do user has set permission status ? */
    if(d_ptr->getStatus() == WlanPerm::WPERM_PROMPT_REQUIRED){
        const WlanError promptStatus = d_ptr->prompt();

        if(promptStatus == WlanError::WERR_NO_ERROR){
            return promptStatus; // Return only if supported, otherwise open parameters
        }
    }

    /* Open parameter windows */
    return d_ptr->openParams();
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
