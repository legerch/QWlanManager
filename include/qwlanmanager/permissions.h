#ifndef QWLANMAN_PERMISSIONS_H
#define QWLANMAN_PERMISSIONS_H

#include "qwlanmanager/qwlanman_global.h"
#include "qwlanmanager/qwlantypes.h"

#include <QObject>

#include <memory>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class PermissionsPrivate;
class QWLANMAN_EXPORT Permissions : public QObject
{
    Q_OBJECT
    QWLANMAN_DISABLE_COPY_MOVE(Permissions)

public:
    explicit Permissions(QObject *parent = nullptr);
    ~Permissions();

public:
    WlanPerm retrieveWlanPermissions();
    WlanError askWlanPermissions();

signals:
    void sPermissionsChanged(qwm::WlanPerm idPerm);

private:
    std::unique_ptr<PermissionsPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Permissions)
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_PERMISSIONS_H
