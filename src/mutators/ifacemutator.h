#ifndef QWLANMAN_MUTATOR_IFACEMUTATOR_H
#define QWLANMAN_MUTATOR_IFACEMUTATOR_H

#include "qwlanmanager/interface.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class IfaceMutator
{

public:
    explicit IfaceMutator(const Interface& iface);

public:
    void setUid(const QUuid& uid);
    void setHwAddress(const QString& hwAddr);
    void setName(const QString& name);
    void setDescription(const QString& desc);
    void setIsUp(bool isUp);

    MapNetworks& getMapNetworksRef();

private:
    Interface m_iface;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_MUTATOR_IFACEMUTATOR_H
