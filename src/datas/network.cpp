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
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
