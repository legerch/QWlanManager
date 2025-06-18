#ifndef QWLANMAN_MANAGER_PRIV_H
#define QWLANMAN_MANAGER_PRIV_H

#include "mutators/ifacemutator.h"
#include "qwlanmanager/manager.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class ManagerPrivate
{
    QWLANMAN_DISABLE_COPY(ManagerPrivate)
    Q_DECLARE_PUBLIC(Manager)

public:
    explicit ManagerPrivate(Manager *parent);

    virtual ~ManagerPrivate();

public:
    virtual void initialize() = 0;
    virtual void terminate() = 0;

public:
    virtual void refreshInterfaces() = 0;

protected:
    MapInterfaces m_interfaces;

    Manager *q_ptr;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_MANAGER_PRIV_H
