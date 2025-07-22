#include "qwlanmanager/manager.h"

#include "factorybackend.h"
#include "manager_priv.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class qwm::Manager
 *
 * \brief Use to manage all \b wlan related operations
 * \details
 * This class control all interfaces operations: interface added/removed,
 * scan, connection, etc...
 *
 * \note
 * Also note that some OSes require permissions to be granted to perform
 * \c wlan operations, see qwm::Permissions for more details.
 *
 * \sa qwm::Permissions
 */

/*****************************/
/* Signals documentations    */
/*****************************/

/*!
 * \fn Manager::sInterfaceAdded()
 * \brief Emitted when a new interface has been added.
 *
 * \param[in] interface
 * Interface that has been added
 *
 * \sa sInterfaceRemoved()
 */

/*!
 * \fn Manager::sInterfaceRemoved()
 * \brief Emitted when an interface has been removed.
 *
 * \param[in] interface
 * Interface that has been removed
 *
 * \sa sInterfaceAdded()
 */

/*!
 * \fn Manager::sScanStarted()
 * \brief Emitted when a network scan has been started on an interface.
 *
 * \param[in] idInterface
 * Interface ID for which the scan has been started.
 *
 * \sa sScanSucceed(), sScanFailed()
 * \sa doScan()
 */

/*!
 * \fn Manager::sScanSucceed()
 * \brief Emitted when a network scan succeeded.
 *
 * \param[in] idInterface
 * Interface ID for which the scan succeeded.
 * \param[in] listNets
 * List of networks found.
 *
 * \sa sScanFailed()
 * \sa sScanStarted()
 * \sa doScan()
 */

/*!
 * \fn Manager::sScanFailed()
 * \brief Emitted when a network scan failed.
 *
 * \param[in] idInterface
 * Interface ID for which the scan failed.
 * \param[in] idErr
 * Error ID explaining why scan has failed.
 *
 * \sa sScanSucceed()
 * \sa sScanStarted()
 * \sa doScan()
 */

/*!
 * \fn Manager::sConnectionStarted()
 * \brief Emitted when trying to connect to a network.
 *
 * \param[in] idInterface
 * Interface ID used.
 * \param[in] ssid
 * Network SSID the interface is trying to connect
 *
 * \sa sConnectionSucceed(), sConnectionFailed()
 * \sa doConnect()
 */

/*!
 * \fn Manager::sConnectionSucceed()
 * \brief Emitted when network connection succeed.
 *
 * \param[in] idInterface
 * Interface ID for which the connection succeeded.
 * \param[in] ssid
 * Network SSID connected
 *
 * \sa sConnectionFailed()
 * \sa sConnectionStarted()
 * \sa doConnect()
 */

/*!
 * \fn Manager::sConnectionFailed()
 * \brief Emitted when network connection failed.
 *
 * \param[in] idInterface
 * Interface ID for which the connection failed.
 * \param[in] ssid
 * Network SSID interface was trying to connect
 * \param[in] idErr
 * Error ID explaining why connection has failed.
 *
 * \sa sConnectionSucceed()
 * \sa sConnectionStarted()
 * \sa doConnect()
 */

/*!
 * \fn Manager::sDisconnectionStarted()
 * \brief Emitted when interface is trying to disconnect from network.
 *
 * \param[in] idInterface
 * Interface ID used.
 *
 * \sa sDisconnectionSucceed(), sDisconnectionFailed()
 * \sa doDisconnect()
 */

/*!
 * \fn Manager::sDisconnectionSucceed()
 * \brief Emitted when interface disconnection succeeded.
 *
 * \param[in] idInterface
 * Interface ID used.
 *
 * \sa sDisconnectionFailed()
 * \sa sDisconnectionStarted()
 * \sa doDisconnect()
 */

/*!
 * \fn Manager::sDisconnectionFailed()
 * \brief Emitted when interface disconnection failed.
 *
 * \param[in] idInterface
 * Interface ID used.
 * \param[in] idErr
 * Error ID explaining why disconnection has failed.
 *
 * \sa sDisconnectionSucceed()
 * \sa sDisconnectionStarted()
 * \sa doDisconnect()
 */

/*!
 * \fn Manager::sForgetStarted()
 * \brief Emitted when trying to forget a network.
 *
 * \param[in] idInterface
 * Interface ID used.
 * \param[in] ssid
 * Network SSID to forget
 *
 * \sa sForgetSucceed(), sForgetFailed()
 * \sa doForget()
 */

/*!
 * \fn Manager::sForgetSucceed()
 * \brief Emitted when network has been forgotten.
 *
 * \param[in] idInterface
 * Interface ID used.
 * \param[in] ssid
 * Network SSID forgotten
 *
 * \sa sForgetFailed()
 * \sa sForgetStarted()
 * \sa doForget()
 */

/*!
 * \fn Manager::sForgetFailed()
 * \brief Emitted when network forget request failed.
 *
 * \param[in] idInterface
 * Interface ID used.
 * \param[in] ssid
 * Network SSID to forgot
 * \param[in] idErr
 * Error ID explaining failure reasons.
 *
 * \sa sForgetSucceed()
 * \sa sForgetStarted()
 * \sa doForget()
 */

/*!
 * \fn Manager::sSignalQualityChanged()
 * \brief Emitted when signal quality of current network changes.
 *
 * \param[in] idInterface
 * Interface ID for which the quality signal has changed.
 * \param[in] percent
 * Signal quality in percentage.
 *
 * \note
 * This signal is only emitted for current network of the interface. \n
 * Other networks quality signals can be retrieved via \c doScan()
 *
 * \sa doScan(), doConnect()
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
 * \brief Construct a wlan manager
 * \details
 * Allow to perform needed backend initialization and retrieve list
 * of available interfaces.
 *
 * \param[out] parent
 * The parent argument is passed to QObject's constructor
 */
Manager::Manager(QObject *parent) :
    QObject{parent},
    d_ptr(FactoryBackend::createEngine(this))
{
    d_ptr->initialize();
    d_ptr->interfaceListUpdate();
}

Manager::~Manager()
{
    d_ptr->terminate();
}

/*!
 * \brief Allow to perform a scan
 * \details
 * This request is asynchronous, use associated signals
 * to manage status.
 *
 * \param[in] idInterface
 * Interface ID to use.
 *
 * \sa sScanStarted()
 * \sa sScanSucceed(), sScanFailed()
 */
void Manager::doScan(const QUuid &idInterface)
{
    /* Emit start signal */
    emit sScanStarted(idInterface);

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

/*!
 * \brief Allow to connect to a network
 * \details
 * This request is asynchronous, use associated signals
 * to manage status.
 *
 * \param[in] idInterface
 * Interface ID to use.
 * \param[in] ssid
 * Network SSID to connect. \n
 * Nothing is performed if already connected.
 * \param[in] password
 * Password to use for connection. \n
 * This value can be empty, in this case, internal credentials
 * will be used. \n
 * If the password isn't registered internally or is wrong, signal
 * \c sConnectionFailed() will be emitted with error code
 * \c WlanError::WERR_NET_PASSKEY: in this case password \b has
 * to be provided.
 *
 * \note
 * Recommended scenario is to call this method once with empty password
 * and ask the user for the password only if \c WlanError::WERR_NET_PASSKEY
 * error is received.
 *
 * \sa sConnectionStarted()
 * \sa sConnectionSucceed(), sConnectionFailed()
 * \sa doDisconnect(), doForget()
 */
void Manager::doConnect(const QUuid &idInterface, const QString &ssid, const QString &password)
{
    /* Emit start signal */
    emit sConnectionStarted(idInterface, ssid);

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

/*!
 * \brief Allow to disconnect from a network
 * \details
 * This request is asynchronous, use associated signals
 * to manage status.
 *
 * \param[in] idInterface
 * Interface ID to use. \n
 * Nothing is performed if already disconnected.
 *
 * \sa sDisconnectionStarted()
 * \sa sDisconnectionSucceed(), sDisconnectionFailed()
 * \sa doForget(), doConnect()
 */
void Manager::doDisconnect(const QUuid &idInterface)
{
    /* Emit start signal */
    emit sDisconnectionStarted(idInterface);

    /* Retrieve associated interface */
    Interface iface = getInterface(idInterface);
    if(!iface.isValid()){
        qWarning("Unable to perform disconnection, unknown interface ID [uuid: %s]", qUtf8Printable(idInterface.toString()));
        emit sDisconnectionFailed(idInterface, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Start disconnection request */
    d_ptr->interfaceDisconnect(iface);
}

/*!
 * \brief Allow to remove network internal credentials
 * \details
 * This request is asynchronous, use associated signals
 * to manage status.
 *
 * \param[in] idInterface
 * Interface ID to use.
 * \param[in] ssid
 * Network SSID to forget
 *
 * \note
 * Using this method will trigger a disconnection.
 *
 * \sa sForgetStarted()
 * \sa sForgetSucceed(), sForgetFailed()
 */
void Manager::doForget(const QUuid &idInterface, const QString &ssid)
{
    /* Emit start signal */
    emit sForgetStarted(idInterface, ssid);

    /* Retrieve associated interface */
    Interface iface = getInterface(idInterface);
    if(!iface.isValid()){
        qWarning("Unable to forget network, unknown interface ID [uuid: %s]", qUtf8Printable(idInterface.toString()));
        emit sForgetFailed(idInterface, ssid, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Retrieve associated network */
    Network net = iface.getNetwork(ssid);
    if(!net.isValid()){
        qWarning("Unable to forget network, unknown network SSID [ssid: %s]", qUtf8Printable(ssid));
        emit sForgetFailed(idInterface, ssid, WlanError::WERR_ITEM_INVALID);
        return;
    }

    /* Start connection request */
    d_ptr->interfaceForget(iface, net);
}

/*!
 * \brief Allow to retrieve list of interfaces
 * \return
 * Returns list of available interfaces.
 *
 * \sa getInterface()
 * \sa sInterfaceAdded(), sInterfaceRemoved()
 */
ListInterfaces Manager::getInterfaces() const
{
    return d_ptr->m_currentIfaces.values();
}

/*!
 * \brief Allow to retrieve an interface via its ID
 *
 * \param[in] idInterface
 * Interface ID to use. \n
 * If ID is unknown, returned interface will be invalid.
 *
 * \return
 * Returns interface related to the ID.
 *
 * \sa getInterfaces()
 * \sa Interface::isValid()
 */
Interface Manager::getInterface(const QUuid &idInterface) const
{
    return d_ptr->m_currentIfaces.value(idInterface);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
