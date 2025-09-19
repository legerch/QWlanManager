#ifndef QWLANMAN_BACKEND_NM_DEVICEINFOPROVIDER_H
#define QWLANMAN_BACKEND_NM_DEVICEINFOPROVIDER_H

extern "C" {
#include <pci/pci.h>
}

#include <QString>
#include <QVariant>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/* DevicePci                 */
/*****************************/

class DevicePci
{
public:
    DevicePci();
    ~DevicePci();

public:
    QString getVendorName(uint vendorId) const;
    QString getDeviceName(uint vendorId, uint deviceId) const;

private:
    struct pci_access *m_pcia = nullptr;
};

/*****************************/
/* Class definitions         */
/* DeviceInfosProvider       */
/*****************************/

class DeviceInfosProvider
{
public:
    DeviceInfosProvider();
    ~DeviceInfosProvider();

public:
    QString retrieveAdapterDesc(const QString &sysfIface);

private:
    QString retrieveSysfPropertyStr(const QString &sysfIface, const QString &property) const;
    uint retrieveSysfPropertyUint(const QString &sysfIface, const QString &property) const;

private:
    DevicePci m_pci;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_NM_DEVICEINFOPROVIDER_H
