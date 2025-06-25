#ifndef QWLANMAN_DATAS_NETWORKMUTATOR_H
#define QWLANMAN_DATAS_NETWORKMUTATOR_H

#include "qwlanmanager/datas/network.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class NetworkMutator
{
public:
    explicit NetworkMutator(const Network &network);

public:
    void setSsid(const QString &ssid);
    void setProfileName(const QString &profile);
    void setAuthAlgo(AuthAlgo idAuth);
    void setCipherAlgo(CipherAlgo idCipher);

private:
    Network m_network;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_NETWORKMUTATOR_H
