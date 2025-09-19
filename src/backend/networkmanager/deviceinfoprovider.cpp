#include "deviceinfoprovider.h"

#include <QFile>

/*****************************/
/* Macro definitions         */
/*****************************/

#define BUFFER_SIZE 512

/*****************************/
/* Class documentations      */
/*****************************/

//TODO: doc
// https://man7.org/linux/man-pages/man7/pcilib.7.html

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/
namespace qwm
{

/*****************************/
/* Constants definitions     */
/*****************************/

/*****************************/
/* Functions implementation  */
/* DevicePci                 */
/*****************************/

DevicePci::DevicePci()
{
    /* Initialize PCI structure */
    m_pcia = pci_alloc();
    pci_init(m_pcia);

    /* Load ressources */
    pci_load_name_list(m_pcia);
}

DevicePci::~DevicePci()
{
    pci_cleanup(m_pcia);
    m_pcia = nullptr;
}

QString DevicePci::getVendorName(uint vendorId) const
{
    char name[BUFFER_SIZE];

    /* Retrieve vendor name */
    const char *vname = pci_lookup_name(m_pcia, name, BUFFER_SIZE, PCI_LOOKUP_VENDOR, vendorId);
    if(!vname){
        return QString("Unknown vendor (0x%1)").arg(vendorId, 4, 16, QChar('0'));
    }

    /* Convert buffer */
    return QString::fromLocal8Bit(name);
}

QString DevicePci::getDeviceName(uint vendorId, uint deviceId) const
{
    char name[BUFFER_SIZE];

    /* Retrieve vendor name */
    const char *vname = pci_lookup_name(m_pcia, name, BUFFER_SIZE, PCI_LOOKUP_DEVICE, vendorId, deviceId);
    if(!vname){
        return QString("Unknown device (0x%1)").arg(deviceId, 4, 16, QChar('0'));
    }

    /* Convert buffer */
    return QString::fromLocal8Bit(name);
}

/*****************************/
/* Functions implementation  */
/* DeviceInfosProvider       */
/*****************************/

DeviceInfosProvider::DeviceInfosProvider()
{
    /* Nothing to do */
}

DeviceInfosProvider::~DeviceInfosProvider()
{
    /* Nothing to do */
}

QString DeviceInfosProvider::retrieveAdapterDesc(const QString &sysfIface)
{
    /* Retrieve device IDs */
    const uint vendorId = retrieveSysfPropertyUint(sysfIface, "vendor");
    const uint deviceId = retrieveSysfPropertyUint(sysfIface, "device");

    if(vendorId == 0 || deviceId == 0){
        return QString();
    }

    /* Convert to human readable string */
    const QString vendor = m_pci.getVendorName(vendorId);
    const QString device = m_pci.getDeviceName(vendorId, deviceId);

    /* Create description */
    return QString("%1 - %2").arg(vendor, device);
}

QString DeviceInfosProvider::retrieveSysfPropertyStr(const QString &sysfIface, const QString &property) const
{
    const QString path = QString("/sys/class/net/%1/device/%2").arg(sysfIface, property);

    /* Open property file */
    QFile file(path);
    bool succeed = file.open(QIODevice::ReadOnly | QIODeviceBase::Text);
    if(!succeed){
        qCritical("Unable to open sysf property at '%s'", qUtf8Printable(path));
        return QString();
    }

    /* Read property */
    const QByteArray data = file.readLine().trimmed();
    return data;
}

uint DeviceInfosProvider::retrieveSysfPropertyUint(const QString &sysfIface, const QString &property) const
{
    QString data = retrieveSysfPropertyStr(sysfIface, property);

    /* Strip potential "0x" prefix */
    if(data.startsWith("0x", Qt::CaseInsensitive)){
        data.remove(0, 2);
    }

    /* Convert to uint (base 16) */
    bool succeed = false;
    const uint value = data.toUInt(&succeed, 16);
    if(!succeed){
        qCritical("Unable to convert sysf data to uint [data: '%s']", qUtf8Printable(data));
        return 0;
    }

    return value;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
