#include "qwlanmanager/datas/network.h"

#include "networkdata.h"

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

Network::Network()
    : d_ptr(QSharedPointer<NetworkData>::create())
{
    /* Nothing to do */
}

Network::Network(const Network &other) = default;
Network::Network(Network &&other) = default;

Network::~Network() = default;

bool Network::isValid() const
{
    if(d_ptr.isNull()){
        return false;
    }

    return !getSsid().isEmpty();
}

const QString& Network::getSsid() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_ssid;
}

const QString& Network::getProfileName() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_profileName;
}

AuthAlgo Network::getAuthAlgo() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_idAuth;
}

CipherAlgo Network::getCipherAlgo() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_idCipher;
}

uint Network::getSignalQuality() const
{
    QMutexLocker locker(&d_ptr->m_mutex);
    return d_ptr->m_signalQuality;
}

Network& Network::operator=(const Network &other) = default;
Network& Network::operator=(Network &&other) = default;

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
