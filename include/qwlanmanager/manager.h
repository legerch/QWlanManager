#ifndef QWLANMAN_MANAGER_H
#define QWLANMAN_MANAGER_H

#include "qwlanmanager/qwlanman_global.h"

#include "qwlanmanager/datas/interface.h"
#include "qwlanmanager/qwlantypes.h"

#include <QObject>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class ManagerPrivate;
class QWLANMAN_EXPORT Manager : public QObject
{
    Q_OBJECT
    QWLANMAN_DISABLE_COPY_MOVE(Manager)

public:
    explicit Manager(QObject *parent = nullptr);
    ~Manager();

public:
    void doScan(const QUuid &idInterface);

public:
    ListInterfaces getInterfaces() const;
    Interface getInterface(const QUuid &idInterface) const;

signals:
    void sInterfaceAdded(qwm::Interface interface);
    void sInterfaceRemoved(qwm::Interface interface);

    void sScanSucceed(const QUuid &idInterface, qwm::ListNetworks listNets);
    void sScanFailed(const QUuid &idInterface, qwm::WlanError idErr);

private:
    std::unique_ptr<ManagerPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Manager)
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_MANAGER_H
