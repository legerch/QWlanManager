#include "deviceinfoprovider.h"

/*****************************/
/* Macro definitions         */
/*****************************/

/*****************************/
/* Class documentations      */
/*****************************/

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
/* DeviceInfos               */
/*****************************/

DeviceInfos::DeviceInfos()
{
    /* Nothing to do */
}

DeviceInfos::~DeviceInfos()
{
    /* Nothing to do */
}

uint DeviceInfos::getIdVendor() const
{
    return m_idVendor;
}

uint DeviceInfos::getIdProduct() const
{
    return m_idProduct;
}

QString DeviceInfos::getNameVendor() const
{
    return m_nameVendor;
}

QString DeviceInfos::getNameProduct() const
{
    return m_nameProduct;
}

void DeviceInfos::setIdVendor(uint newIdVendor)
{
    m_idVendor = newIdVendor;
}

void DeviceInfos::setIdProduct(uint newIdProduct)
{
    m_idProduct = newIdProduct;
}

void DeviceInfos::setNameVendor(const QString &newNameVendor)
{
    m_nameVendor = newNameVendor;
}

void DeviceInfos::setNameProduct(const QString &newNameProduct)
{
    m_nameProduct = newNameProduct;
}

QDebug operator<<(QDebug debug, const DeviceInfos &devInfos)
{
    QDebugStateSaver saver(debug);

    const QString vendor = QString("vendor: '%1' (0x%2)").arg(devInfos.getNameVendor()).arg(devInfos.getIdVendor(), 4, 16, QChar('0'));
    const QString product = QString("product: '%1' (0x%2)").arg(devInfos.getNameProduct()).arg(devInfos.getIdProduct(), 4, 16, QChar('0'));

    debug.nospace() << "DeviceInfos("
                    << vendor << ", "
                    << product << ")";
    return debug;
}

/*****************************/
/* Functions implementation  */
/* HandleUdev                */
/*****************************/

HandleUdev& HandleUdev::instance()
{
    static HandleUdev instance;
    return instance;
}

HandleUdev::HandleUdev()
{
    m_handle = udev_new();
}

HandleUdev::~HandleUdev()
{
    udev_unref(m_handle);
    m_handle = nullptr;
}

/*!
 * \brief HandleUdev::retrieveDeviceInfos
 * \details
 * We can see all availables device informations by using command:
 * \code{.sh}
 * udevadm info /sys/class/net/<iface_name>
 * \endcode
 *
 * \param ifaceName
 * \param devInfos
 * \return
 */
bool HandleUdev::retrieveDeviceInfos(const QString &ifaceName, DeviceInfos &devInfos)
{
    /* Retrieve device */
    struct udev_device *dev = udev_device_new_from_subsystem_sysname(m_handle, "net", ifaceName.toStdString().c_str());
    if(!dev){
        qCritical("Unable to retrieve device informations [iface: '%s']", qUtf8Printable(ifaceName));
        return false;
    }

    /* Retrieve properties */
    devInfos.setIdVendor(retrieveDevicePropertyUint(dev, "ID_VENDOR_ID"));
    devInfos.setIdProduct(retrieveDevicePropertyUint(dev, "ID_MODEL_ID"));

    devInfos.setNameVendor(retrieveDevicePropertyStr(dev, "ID_VENDOR_FROM_DATABASE"));
    devInfos.setNameProduct(retrieveDevicePropertyStr(dev, "ID_MODEL_FROM_DATABASE"));

    /* Clean ressources */
    udev_device_unref(dev);

    return true;
}

QString HandleUdev::retrieveDevicePropertyStr(udev_device *dev, const char *idName) const
{
    const char *value = udev_device_get_property_value(dev, idName);
    return (value ? QString::fromLocal8Bit(value) : QString());
}

uint HandleUdev::retrieveDevicePropertyUint(udev_device *dev, const char *idName) const
{
    QString data = retrieveDevicePropertyStr(dev, idName);

    /* Strip potential "0x" prefix */
    if(data.startsWith("0x", Qt::CaseInsensitive)){
        data.remove(0, 2);
    }

    /* Convert to uint (base 16) */
    bool succeed = false;
    const uint value = data.toUInt(&succeed, 16);
    if(!succeed){
        return 0;
    }

    return value;
}

/*****************************/
/* Functions implementation  */
/* DeviceInfosProvider       */
/*****************************/

QString DeviceInfosProvider::retrieveAdapterDesc(const QString &ifaceName)
{
    DeviceInfos devInfos;
    bool succeed = HandleUdev::instance().retrieveDeviceInfos(ifaceName, devInfos);
    if(!succeed){
        return QString();
    }

    /* Create description */
    QString desc = devInfos.getNameVendor();

    QString productName = devInfos.getNameProduct();
    if(!productName.isEmpty()){
        desc.append(QStringLiteral(" - %1").arg(productName));
    }

    return desc;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
