#include "typeswinnative.h"

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

namespace qwm::WinNative
{

/*!
 * \brief Use to convert \b WlanAPI error to
 * library equivalent error.
 *
 * \param[in] apiErr
 * API error to convert. \n
 * Unknown error will be set to \c WlanError::WERR_NO_ERROR.
 *
 * \note
 * https://learn.microsoft.com/en-us/windows/win32/nativewifi/wlan-reason-code
 *
 * \return
 * Returns library equivalent error.
 */
WlanError convertErrFromApi(WLAN_REASON_CODE apiErr)
{
    WlanError idErr = WlanError::WERR_NO_ERROR;

    switch(apiErr)
    {
        case WLAN_REASON_CODE_USER_CANCELLED:                   idErr = WlanError::WERR_OPERATION_CANCEL;break;
        case WLAN_REASON_CODE_MSMSEC_CANCELLED:                 idErr = WlanError::WERR_OPERATION_CANCEL; break;

        case WLAN_REASON_CODE_NETWORK_NOT_AVAILABLE:            idErr = WlanError::WERR_NET_UNAVAILABLE; break;

        case WLAN_REASON_CODE_KEY_MISMATCH:                     idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PROFILE_KEY_LENGTH:        idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PROFILE_PSK_LENGTH:        idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PSK_MISMATCH_SUSPECTED:    idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_KEY_START_TIMEOUT:         idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_KEY_SUCCESS_TIMEOUT:       idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_KEY_FORMAT:                idErr = WlanError::WERR_NET_PASSKEY; break;
        case WLAN_REASON_CODE_MSMSEC_PROFILE_PASSPHRASE_CHAR:   idErr = WlanError::WERR_NET_PASSKEY; break;

        case WLAN_REASON_CODE_ASSOCIATION_TIMEOUT:              idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_SECURITY_TIMEOUT:                 idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_DISCONNECT_TIMEOUT:               idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_UI_REQUEST_TIMEOUT:               idErr = WlanError::WERR_OPERATION_TIMEOUT; break;

        default:                                                break;
    }

    return idErr;
}

/*!
 * \brief Use to convert authentication algorithm from
 * \c WlanAPI to library equivalent.
 *
 * \param[in] apiAuth
 * API authentication algorithm to convert. \n
 * Unknown or unsupported authentication IDs will
 * be set to \c AuthAlgo::AUTH_ALGO_UNKNOWN.
 *
 * \note
 * https://learn.microsoft.com/en-us/windows/win32/nativewifi/dot11-auth-algorithm
 *
 * \return
 * Returns library equivalent authentication algorithm ID.
 */
AuthAlgo convertAuthFromApi(DOT11_AUTH_ALGORITHM apiAuth)
{
    AuthAlgo idAuth = AuthAlgo::AUTH_ALGO_UNKNOWN;

    switch(apiAuth)
    {
        case DOT11_AUTH_ALGO_80211_OPEN:    idAuth = AuthAlgo::AUTH_ALGO_OPEN; break;

        case DOT11_AUTH_ALGO_RSNA_PSK:      idAuth = AuthAlgo::AUTH_ALGO_WPA2_PERSONAL; break;
        case DOT11_AUTH_ALGO_RSNA:          idAuth = AuthAlgo::AUTH_ALGO_WPA2_ENTERPRISE; break;

        case DOT11_AUTH_ALGO_WPA3_SAE:      idAuth = AuthAlgo::AUTH_ALGO_WPA3_PERSONAL; break;
        case DOT11_AUTH_ALGO_WPA3:          idAuth = AuthAlgo::AUTH_ALGO_WPA3_ENTERPRISE; break;

        default:                            break;
    }

    return idAuth;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm::WinNative

/*****************************/
/* End file                  */
/*****************************/
