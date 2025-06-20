#ifndef QWLANMAN_DATAS_INTERFACEMUTATOR_H
#define QWLANMAN_DATAS_INTERFACEMUTATOR_H

#include "qwlanmanager/datas/interface.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class InterfaceMutator
{

public:
    explicit InterfaceMutator(const Interface &iface);

public:
    void setState(IfaceState idState);
    void setUid(const QUuid &uid);
    void setHwAddress(const QString &hwAddr);
    void setHwAddress(const uchar *buffer, int lenBuffer);
    void setName(const QString &name);
    void setDescription(const QString &desc);

    void setConnectedSsid(const QString &ssid = QString());
    MapNetworks& getMapNetworksRef();

private:
    Interface m_iface;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_INTERFACEMUTATOR_H
