#ifndef QWLANMAN_PERMISSIONS_H
#define QWLANMAN_PERMISSIONS_H

#include "qwlanmanager/qwlanman_global.h"
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

class PermissionsPrivate;
class QWLANMAN_EXPORT Permissions : public QObject
{
    Q_OBJECT
    QWLANMAN_DISABLE_COPY_MOVE(Permissions)

public:
    explicit Permissions(QObject *parent = nullptr);
    ~Permissions();

public:
    WlanPerm wlanRetrieve();
    bool wlanOpenParams();

private:
    std::unique_ptr<PermissionsPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Permissions)
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_PERMISSIONS_H
