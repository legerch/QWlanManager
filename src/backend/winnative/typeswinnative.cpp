#include "typeswinnative.h"

/*****************************/
/* Macro definitions         */
/*****************************/

#define BUFFER_SIZE_MAX 512

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
 * Unknown error will be set to \c WlanError::WERR_UNKNOWN.
 *
 * \note
 * https://learn.microsoft.com/en-us/windows/win32/nativewifi/wlan-reason-code
 *
 * \return
 * Returns library equivalent error.
 */
WlanError convertErrFromApi(WLAN_REASON_CODE apiErr)
{
    WlanError idErr = WlanError::WERR_API_INTERNAL;

    switch(apiErr)
    {
        case WLAN_REASON_CODE_SUCCESS:                          idErr = WlanError::WERR_NO_ERROR; break;

        case WLAN_REASON_CODE_USER_CANCELLED:
        case WLAN_REASON_CODE_MSMSEC_CANCELLED:                 idErr = WlanError::WERR_OPERATION_CANCEL; break;

        case WLAN_REASON_CODE_NETWORK_NOT_AVAILABLE:            idErr = WlanError::WERR_NET_UNAVAILABLE; break;

        case WLAN_REASON_CODE_KEY_MISMATCH:
        case WLAN_REASON_CODE_MSMSEC_PROFILE_KEY_LENGTH:
        case WLAN_REASON_CODE_MSMSEC_PROFILE_PSK_LENGTH:
        case WLAN_REASON_CODE_MSMSEC_PROFILE_PASSPHRASE_CHAR:
        case WLAN_REASON_CODE_MSMSEC_PSK_MISMATCH_SUSPECTED:
        case WLAN_REASON_CODE_MSMSEC_KEY_START_TIMEOUT:
        case WLAN_REASON_CODE_MSMSEC_KEY_SUCCESS_TIMEOUT:
        case WLAN_REASON_CODE_MSMSEC_KEY_FORMAT:                idErr = WlanError::WERR_NET_PASSKEY; break;

        case WLAN_REASON_CODE_ASSOCIATION_TIMEOUT:              idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_SECURITY_TIMEOUT:                 idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_DISCONNECT_TIMEOUT:               idErr = WlanError::WERR_OPERATION_TIMEOUT; break;
        case WLAN_REASON_CODE_UI_REQUEST_TIMEOUT:               idErr = WlanError::WERR_OPERATION_TIMEOUT; break;

        default:{
            // Convert error to string
            WCHAR buffer[BUFFER_SIZE_MAX];
            const DWORD res = WlanReasonCodeToString(apiErr, BUFFER_SIZE_MAX, buffer, nullptr);

            // Manage result
            QString strErr = "none";
            if(res == ERROR_SUCCESS){
                strErr = QString::fromWCharArray(buffer);
            }

            qWarning("Unable to convert error ID from API [id: 0x%08X, string: '%s']", apiErr, qUtf8Printable(strErr));
        }break;
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

        default:                            qWarning("Unable to convert authentication algorithm from API [id: %d]", apiAuth); break;
    }

    return idAuth;
}

/*!
 * \brief Use to convert cipher algorithm from
 * \c WlanAPI to library equivalent.
 *
 * \param[in] apiCipher
 * API cipher algorithm to convert. \n
 * Unknown or unsupported cipher IDs will
 * be set to \c CipherAlgo::CIPHER_ALGO_UNKNOWN.
 *
 * \note
 * https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/wlantypes/ne-wlantypes-_dot11_cipher_algorithm
 *
 * \return
 * Returns library equivalent cipher algorithm ID.
 */
CipherAlgo convertCipherFromApi(DOT11_CIPHER_ALGORITHM apiCipher)
{
    CipherAlgo idCipher = CipherAlgo::CIPHER_ALGO_UNKNOWN;

    switch(apiCipher)
    {
        case DOT11_CIPHER_ALGO_NONE:        idCipher = CipherAlgo::CIPHER_ALGO_NONE; break;

        case DOT11_CIPHER_ALGO_TKIP:        idCipher = CipherAlgo::CIPHER_ALGO_TKIP; break;

        case DOT11_CIPHER_ALGO_CCMP_256:
        case DOT11_CIPHER_ALGO_CCMP:        idCipher = CipherAlgo::CIPHER_ALGO_AES; break;

        case DOT11_CIPHER_ALGO_GCMP:
        case DOT11_CIPHER_ALGO_GCMP_256:    idCipher = CipherAlgo::CIPHER_ALGO_GCMP; break;

        default:                            qWarning("Unable to convert cipher algorithm from API [id: %d]", apiCipher); break;
    }

    return idCipher;
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm::WinNative

/*****************************/
/* End file                  */
/*****************************/
