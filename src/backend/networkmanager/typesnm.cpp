#include "typesnm.h"

/*****************************/
/* Macro definitions         */
/*****************************/

/*****************************/
/* Namespace documentations  */
/*****************************/

/*!
 * \namespace qwm::NmBus
 * \brief Namespace used to manage types
 * from <b>Network Manager DBUS</b> API.
 * \details
 * For more details, see:
 * - https://networkmanager.dev/docs/api/latest/index.html
 */

/*****************************/
/* Class documentations      */
/*****************************/

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm::NmBus
{

/*!
 * \brief Use to determine if the device is
 * a wireless IEEE 802.11 interface or not.
 * \details
 * Refer to the official doumentation for more details:
 * - https://networkmanager.dev/docs/api/latest/nm-dbus-types.html#NMDeviceType
 *
 * \param[in] idType
 * Type ID to verify.
 *
 * \return
 * Returns \c true if interface is a 802.11
 * wifi device.
 */
bool isWifiDevice(int idType)
{
    constexpr int NM_DEVICE_TYPE_WIFI = 2;
    return idType == NM_DEVICE_TYPE_WIFI;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm::NmBus

/*****************************/
/* End file                  */
/*****************************/
