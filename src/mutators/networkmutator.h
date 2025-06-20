#ifndef QWLANMAN_MUTATOR_NETWORKMUTATOR_H
#define QWLANMAN_MUTATOR_NETWORKMUTATOR_H

#include "qwlanmanager/network.h"

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

private:
    Network m_network;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_MUTATOR_NETWORKMUTATOR_H
