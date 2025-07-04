#ifndef QWLANMAN_DATAS_NETWORKMUTATOR_H
#define QWLANMAN_DATAS_NETWORKMUTATOR_H

#include "cache/cacheinfo.h"
#include "qwlanmanager/datas/network.h"

#include <any>

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
    void setSignalQuality(uint quality);

    CacheInfo& getCacheRef();

    void setDataEngine(const std::any &data);
    const std::any& getDataEngine() const;

private:
    Network m_network;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_NETWORKMUTATOR_H
