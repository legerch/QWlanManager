#ifndef QWLANMAN_QWLANTYPES_H
#define QWLANMAN_QWLANTYPES_H

#include "qwlanman_global.h"

#include <QObject>
#include <QVersionNumber>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{
    Q_NAMESPACE_EXPORT(QWLANMAN_EXPORT)

/*****************************/
/* Library types             */
/*****************************/

    /*!
     * \brief List of all WLAN related errors
     *
     * \sa wlanErrorToString()
     */
    enum class WlanError
    {
        WERR_NO_ERROR = 0,          /**< No error, operation succeed */
        WERR_UNKNOWN,               /**< Unknown error */

        WERR_OPERATION_UNSUPPORTED, /**< Operation is not supported on this platform version */
        WERR_OPERATION_CANCEL,      /**< Operation has been cancelled */
        WERR_OPERATION_TIMEOUT,     /**< Operation failure due to timeout */
        WERR_ITEM_INVALID,          /**< Operation cannot be performed due to an invalid item */
        WERR_IFACE_BUSY,            /**< Interface is already busy, cannot perform. Setting option \c IfaceOption::IFACE_OPT_REQUEST will help to get ride of this error */
        WERR_NET_UNAVAILABLE,       /**< Trying to connect to an unailable network */
        WERR_NET_PASSKEY,           /**< Provided passkey is invalid */
        WERR_API_INTERNAL           /**< Triggered by internal API errors, please refer to logs for more details */
    };
    Q_ENUM_NS(WlanError)

    QWLANMAN_EXPORT QString wlanErrorToString(WlanError idErr);

    /*!
     * \brief List of permissions status
     *
     * \sa wlanPermToString()
     * \sa Permissions
     */
    enum class WlanPerm
    {
        WPERM_UNKNOWN = 0,      /**< Permission unknown, either because not fetched yet or platform doesn't support the operation */

        WPERM_PROMPT_REQUIRED,  /**< User didn't deny or grant permission yet */
        WPERM_GRANTED,          /**< Wlan permissions are granted */
        WPERM_DENIED            /**< Wlan permissions has been denied */
    };
    Q_ENUM_NS(WlanPerm)

    QWLANMAN_EXPORT QString wlanPermToString(WlanPerm idPerm);

    /*!
     * \brief Type of interface asynchrone request
     *
     * \sa requestTypeToString()
     */
    enum class RequestType
    {
        REQ_INVALID = 0,    /**< Invalid request */

        REQ_SCAN,           /**< Scan request */
        REQ_CONNECT,        /**< Connection request */
        REQ_DISCONNECT      /**< Disconnection request */
    };
    Q_ENUM_NS(RequestType);

    QWLANMAN_EXPORT QString requestTypeToString(RequestType idReq);

    /*!
     * \brief Interface states
     *
     * \sa ifaceStateToString()
     */
    enum class IfaceState
    {
        IFACE_STS_IDLE = 0,         /**< Interface ready to perform an operation */
        IFACE_STS_SCANNING,         /**< Interface is performing a network scan */
        IFACE_STS_CONNECTING,       /**< Interface is trying to connect to a network */
        IFACE_STS_DISCONNECTING     /**< Interface is trying to disconnect from a network */
    };
    Q_ENUM_NS(IfaceState);

    QWLANMAN_EXPORT QString ifaceStateToString(IfaceState idState);

    /*!
     * \brief Available interface options
     * \details
     * Options are flags, so they can be combined.
     */
    enum IfaceOption
    {
        IFACE_OPT_NONE      = 0,        /**< No option set */

        IFACE_OPT_REQUEST   = 1 << 0    /**< Allow to queue interface operations (wait for the scan to finish before trying to connect for example), which
                                            will reduce \c WlanError::WERR_IFACE_BUSY occurrences. \n
                                            Note that enabling this will delay operations start.
                                        */
    };
    Q_DECLARE_FLAGS(IfaceOptions, IfaceOption)
    Q_FLAG_NS(IfaceOptions)

    /*!
     * \brief Enumeration of supported wireless authentication algorithms.
     * \details
     * Represents the authentication mechanism used by a wireless network. \n
     * It reflects the mechanism advertised in the RSN (Robust Security Network) Information Element
     * of a beacon or probe response frame, as defined by the IEEE 802.11 standards (802.11i, 802.11r, 802.11ax, etc.).
     *
     * \note
     * These enums are focused on client-side support (i.e., supplicant) and align with typical usage
     * scenarios such as connecting to Wi-Fi networks in WPA2/WPA3 Personal or Enterprise modes.
     *
     * \sa authAlgoToString()
     */
    enum class AuthAlgo
    {
        AUTH_ALGO_UNKNOWN = 0,      /**< Authentication algorithm is unknown (thus unsupported) */

        AUTH_ALGO_OPEN,             /**< Open System authentication, defined in IEEE 802.11-1997).\n
                                         No authentication is required to join the network */

        AUTH_ALGO_WPA2_PERSONAL,    /**< WPA2-Personal (also known as WPA2-PSK), defined in IEEE 802.11i-2004.\n
                                     Uses a pre-shared key (PSK) for authentication */
        AUTH_ALGO_WPA2_ENTERPRISE,  /**< WPA2-Enterprise, defined in IEEE 802.11i-2004. \n
                                        Uses IEEE 802.1X/EAP for authentication instead of PSK.
                                        Requires a RADIUS server and per-user credentials (e.g., username/password or certificate). \n
                                        Used in corporate and academic environments. */

        AUTH_ALGO_WPA3_PERSONAL,    /**< WPA3-Personal (defined in Wi-Fi CERTIFIED WPA3™ standard, based on IEEE 802.11ax and SAE).
                                        Uses SAE (Simultaneous Authentication of Equals) for mutual authentication */
        AUTH_ALGO_WPA3_ENTERPRISE   /**< WPA3-Enterprise (also part of the WPA3 standard, optional enhancements in IEEE 802.11-2020). \n
                                        Similar to WPA2-Enterprise but may enforce stronger cryptographic requirements. \n
                                        Requires an 802.1X/EAP infrastructure. */
    };
    Q_ENUM_NS(AuthAlgo);

    QWLANMAN_EXPORT QString authAlgoToString(AuthAlgo idAuth);

    /*!
     * \brief Enumeration of supported wireless encryption (cipher) algorithms.
     *
     * \sa cipherAlgoToString()
     */
    enum class CipherAlgo
    {
        CIPHER_ALGO_UNKNOWN = 0,    /**< Cipher algorithm is unknown (thus unsupported) */

        CIPHER_ALGO_NONE,           /**< No encryption used, typically used with \c AuthAlgo::AUTH_ALGO_OPEN networks */
        CIPHER_ALGO_TKIP,           /**< Temporal Key Integrity Protocol (TKIP), often considered as deprecated */
        CIPHER_ALGO_AES,            /**< Advanced Encryption Standard (AES) with CCMP mode (Counter Mode CBC-MAC Protocol). Default cipher for WPA2 and WPA3 networks. */
        CIPHER_ALGO_GCMP            /**< Galois/Counter Mode Protocol (GCMP), introduced in 802.11ac and required for WPA3-Enterprise 192-bit mode. Typically used with WPA3 networks requiring 256-bit keys (AES-GCMP-256). */
    };
    Q_ENUM_NS(CipherAlgo);

    QWLANMAN_EXPORT QString cipherAlgoToString(CipherAlgo idCipher);

/*****************************/
/* Library methods           */
/*****************************/

    QWLANMAN_EXPORT QVersionNumber getLibraryVersion();

/*****************************/
/* Qt related methods        */
/*****************************/

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

inline uint qHash(qwm::WlanError key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::WlanError>::type>(key), seed);
}

inline uint qHash(qwm::WlanPerm key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::WlanPerm>::type>(key), seed);
}

inline uint qHash(qwm::RequestType key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::RequestType>::type>(key), seed);
}

inline uint qHash(qwm::IfaceState key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::IfaceState>::type>(key), seed);
}

inline uint qHash(qwm::AuthAlgo key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::AuthAlgo>::type>(key), seed);
}

inline uint qHash(qwm::CipherAlgo key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::CipherAlgo>::type>(key), seed);
}

#endif

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

/*****************************/
/* Qt specific meta-system   */
/*****************************/

Q_DECLARE_OPERATORS_FOR_FLAGS(qwm::IfaceOptions)

#endif // QWLANMAN_QWLANTYPES_H
