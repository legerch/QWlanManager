#include "qwlanmanager/datas/interface.h"

#include "interfacedata.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class qwm::Interface
 *
 * \brief Class containing interface properties
 * \details
 * This class contains all related interface properties (status, name, list
 * of networks, etc...). \n
 * This class use <b>implicit sharing</b> memory (via <em>shared pointer</em>), this allow to:
 * - Always have an interface object with "last" known properties without having to fetch them from `qwm::Manager`
 * - When setting properties, they are propagated to all instances
 * - Passing this class by value is cheap
 * This class is also compatible with \b QML.
 *
 * \sa Manager::getInterfaces()
 *
 * \cond INTERNAL
 *
 * \note
 * To modify internal properties of this object,
 * use the class \c qwm::InterfaceMutator
 *
 * \endcond
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

Interface::Interface()
    : d_ptr(QSharedPointer<InterfaceData>::create())
{
    /* Nothing to do */
}

Interface::Interface(const Interface &other) = default;
Interface::Interface(Interface &&other) = default;

Interface::~Interface() = default;

/*!
 * \brief Allow to know if an interface instance is valid
 *
 * \return
 * Returns \c true if interface is valid
 */
bool Interface::isValid() const
{
    if(d_ptr.isNull()){
        return false;
    }

    return !getUid().isNull();
}

/*!
 * \brief Allow to know if an interface is busy
 *
 * \return
 * Returns \c true if interface is currently
 * performing a request.
 */
bool Interface::isBusy() const
{
    return getState() != IfaceState::IFACE_STS_IDLE;
}

/*!
 * \brief Retrieve current interface state
 *
 * \return
 * Returns state of interface
 */
IfaceState Interface::getState() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_state;
}

/*!
 * \brief Retrieve current interface options
 *
 * \return
 * Returns options of interface
 */
IfaceOptions Interface::getOptions() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_opts;
}

/*!
 * \brief Retrieve interface Universally
 * Unique Identifier (UUID)
 *
 * \return
 * Returns UUID of the interface
 */
const QUuid& Interface::getUid() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_uid;
}

/*!
 * \brief Retrieve hardware address of the interface
 *
 * \return
 * Returns hardware address of the interface. \n
 * The hardware address will be formatted as:
 * \c AA:11:BB:22:CC:33
 */
const QString& Interface::getHwAddress() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_hwAddress;
}

/*!
 * \brief Retrieve interface name
 *
 * \return
 * Returns name of the interface
 */
const QString& Interface::getName() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_name;
}

/*!
 * \brief Retrieve interface description
 *
 * \return
 * Returns description of the interface
 */
const QString& Interface::getDescription() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_description;
}

/*!
 * \brief Retrieve interface cache policy
 *
 * \return
 * Returns cache policy of the interface
 */
const CachePolicy& Interface::getCachePolicy() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_cachePolicy;
}

/*!
 * \brief Retrieve all networks that interface
 * has detected.
 *
 * \return
 * Returns maps of available networks.
 *
 * \sa getListNetworks()
 * \sa Manager::doScan()
 */
const MapNetworks& Interface::getMapNetworks() const
{
    return d_ptr->m_mapNets;
}

/*!
 * \brief Retrieve all networks that interface
 * has detected.
 *
 * \return
 * Returns list of available networks.
 *
 * \sa getMapNetworks()
 * \sa Manager::doScan()
 */
ListNetworks Interface::getListNetworks() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_mapNets.values();
}

/*!
 * \brief Allow to retrieve a network via SSID
 *
 * \param[in] ssid
 * Network SSID to use. \n
 * If SSID is unknown/unavailable, returned network will be invalid.
 *
 * \return
 * Returns network related to the SSID.
 *
 * \sa getNetworkConnected(), getMapNetworks(), getListNetworks()
 * \sa Network::isValid()
 */
Network Interface::getNetwork(const QString &ssid) const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_mapNets.value(ssid);
}

/*!
 * \brief Allow to retrieve interface current network
 *
 * \return
 * Returns current network of the interface.
 * If interface is not connected, returned network will be invalid.
 *
 * \sa getNetwork(), getMapNetworks(), getListNetworks()
 * \sa Network::isValid()
 */
Network Interface::getNetworkConnected() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_mapNets.value(d_ptr->m_connectedSsid);
}

/*!
 * \brief Allow to set the cache policy of an interface.
 *
 * \param[in] cachePolicy
 * Cache policy to use.
 *
 * \sa setOptions()
 */
void Interface::setCachePolicy(const CachePolicy &cachePolicy)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_cachePolicy = cachePolicy;
}

/*!
 * \brief Allow to set options of an interface.
 *
 * \param[in] opts
 * Options to use.
 *
 * \sa setCachePolicy()
 */
void Interface::setOptions(IfaceOptions opts)
{
    QMutexLocker locker(&d_ptr->m_mutex);
    d_ptr->m_opts = opts;
}

Interface& Interface::operator=(const Interface &other) = default;
Interface& Interface::operator=(Interface &&other) noexcept = default;

/*!
 * \brief Use to compare interface instances
 * \details
 * Interface are considered equals if UUID are equals or if
 * both are invalid.
 *
 * \param[in] other
 * Other interface to compare
 *
 * \return
 * Returns \c true if equals.
 *
 * \sa getUid()
 */
bool Interface::operator==(const Interface &other) const
{
    /* Verify if same pointer */
    if(d_ptr.data() == other.d_ptr.data()){
        return true;
    }

    /* One of the two is null, can't compare */
    if(!d_ptr || !other.d_ptr){
        return false;
    }

    /* Compare content */
    return *d_ptr == *other.d_ptr;
}

bool Interface::operator!=(const Interface &other) const
{
    return !(*this == other);
}

/*****************************/
/* Qt specific methods       */
/*****************************/

QDebug operator<<(QDebug debug, const Interface &interface)
{
    QDebugStateSaver saver(debug);
    if(!interface.isValid()){
        debug.nospace() << "Interface("
                        << "is valid: false)";

        return debug;
    }

    debug.nospace() << "Interface("
                    << "state: " << interface.getState() << ", "
                    << "uid: " << interface.getUid() << ", "
                    << "hw address: " << interface.getHwAddress() << ", "
                    << "name: " << interface.getName() << ", "
                    << "description: " << interface.getDescription() << ","
                    << "options: " << interface.getOptions() << ","
                    << "cache policy: " << interface.getCachePolicy() << ")";
    return debug;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
