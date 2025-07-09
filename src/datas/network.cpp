#include "qwlanmanager/datas/network.h"

#include "networkdata.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*!
 * \class qwm::Network
 *
 * \brief Class containing network properties
 * \details
 * This class contains all related network properties (ssid, authentication algo,
 * signal quality, etc...). \n
 * This class use <b>implicit sharing</b> memory (via <em>shared pointer</em>), this allow to:
 * - Always have a network object with "last" known properties without having to fetch them from `qwm::Interface`
 * - Passing this class by value is cheap
 * This class is also compatible with \b QML.
 *
 * \note
 * From the user point of view, this object is immutable, thus all networks
 * instances will be retrieved via \c qwm::Manager or \c qwm::Interface class.
 *
 * \cond INTERNAL
 *
 * \note
 * To modify this object, use the class qwm::NetworkMutator
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

Network::Network()
    : d_ptr(QSharedPointer<NetworkData>::create())
{
    /* Nothing to do */
}

Network::Network(const Network &other) = default;
Network::Network(Network &&other) = default;

Network::~Network() = default;

/*!
 * \brief Allow to know if a network instance is valid
 *
 * \return
 * Returns \c true if network is valid
 */
bool Network::isValid() const
{
    if(d_ptr.isNull()){
        return false;
    }

    return !getSsid().isEmpty();
}

/*!
 * \brief Retrieve network SSID
 *
 * \return
 * Returns SSID of network
 */
const QString& Network::getSsid() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_ssid;
}

/*!
 * \brief Retrieve network profile name
 *
 * \return
 * Returns profile name of network
 */
const QString& Network::getProfileName() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_profileName;
}

/*!
 * \brief Retrieve network authentication algorithm
 *
 * \return
 * Returns authentication algorithm of network
 */
AuthAlgo Network::getAuthAlgo() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_idAuth;
}

/*!
 * \brief Retrieve network cipher algorithm
 *
 * \return
 * Returns cipher algorithm of network
 */
CipherAlgo Network::getCipherAlgo() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_idCipher;
}

/*!
 * \brief Retrieve network signal quality
 *
 * \return
 * Returns signal quality of network in percent.
 */
uint Network::getSignalQuality() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_signalQuality;
}

Network& Network::operator=(const Network &other) = default;
Network& Network::operator=(Network &&other) = default;

/*!
 * \brief Use to compare network instances
 * \details
 * Networks are considered equals if ssid and
 * authentication/cipher algorithms are equals or if
 * both are invalid.
 *
 * \param[in] other
 * Other network to compare
 *
 * \return
 * Returns \c true if equals.
 */
bool Network::operator==(const Network &other) const
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

bool Network::operator!=(const Network &other) const
{
    return !(*this == other);
}

/*****************************/
/* Qt specific methods       */
/*****************************/

QDebug operator<<(QDebug debug, const Network &net)
{
    QDebugStateSaver saver(debug);
    if(!net.isValid()){
        debug.nospace() << "Network("
                        << "is valid: false)";

        return debug;
    }

    debug.nospace() << "Network("
                    << "ssid: " << net.getSsid() << ", "
                    << "profile name: " << net.getProfileName() << ", "
                    << "auth algo: " << net.getAuthAlgo() << ", "
                    << "cipher algo: " << net.getCipherAlgo() << ","
                    << "signal: " << net.getSignalQuality() << ")";
    return debug;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
