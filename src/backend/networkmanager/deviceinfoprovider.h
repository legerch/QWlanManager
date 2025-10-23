#ifndef QWLANMAN_BACKEND_NM_DEVICEINFOPROVIDER_H
#define QWLANMAN_BACKEND_NM_DEVICEINFOPROVIDER_H

#include "qwlanmanager/qwlanman_global.h"

extern "C" {
#include <libudev.h>
}

#include <QDebug>
#include <QString>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/* DeviceInfos               */
/*****************************/
class DeviceInfos
{
public:
    DeviceInfos();
    ~DeviceInfos();

public:
    uint getIdVendor() const;
    uint getIdProduct() const;
    QString getNameVendor() const;
    QString getNameProduct() const;

public:
    void setIdVendor(uint newIdVendor);
    void setIdProduct(uint newIdProduct);
    void setNameVendor(const QString &newNameVendor);
    void setNameProduct(const QString &newNameProduct);

private:
    uint m_idVendor;
    uint m_idProduct;
    QString m_nameVendor;
    QString m_nameProduct;
};

QDebug operator<<(QDebug debug, const DeviceInfos &devInfos);

/*****************************/
/* Class definitions         */
/* HandleUdev                */
/*****************************/

class HandleUdev final
{
    QWLANMAN_DISABLE_COPY_MOVE(HandleUdev)

public:
    static HandleUdev& instance();

private:
    HandleUdev();
    ~HandleUdev();

public:
    bool retrieveDeviceInfos(const QString &ifaceName, DeviceInfos &devInfos);

private:
    QString retrieveDevicePropertyStr(udev_device *dev, const char *idName) const;
    uint retrieveDevicePropertyUint(udev_device *dev, const char *idName) const;

private:
    struct udev *m_handle = nullptr;
};

/*****************************/
/* Class definitions         */
/* DeviceInfosProvider       */
/*****************************/

class DeviceInfosProvider
{
public:
    static QString retrieveAdapterDesc(const QString &ifaceName);
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_NM_DEVICEINFOPROVIDER_H
