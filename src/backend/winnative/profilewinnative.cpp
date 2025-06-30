#include "profilewinnative.h"

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
/* Functions implementation  */
/*****************************/

ProfileWinNative::ProfileWinNative(const Network &network, const QString &passkey)
    : m_network(network), m_passkey(passkey)
{
    m_name = m_network.getSsid();
}

const QString& ProfileWinNative::getName() const
{
    return m_name;
}

/*!
 * \brief ProfileWinNative::toXmlFormat
 *
 * \warning
 * Don't change "HTTP" by "HTTPS", otherwise profile creation will fail.\n
 * Other similar issues:
 * - https://www.vbforums.com/showthread.php?883153-RESOLVED-WlanSetProfile-returns-Error&s=d98948f7cb6c424ab0fab277d9581302&p=5450321#post5450321
 *
 * \return
 */
QString ProfileWinNative::toXmlFormat() const
{
    /* Escape needed values */
    const QString ssid = m_network.getSsid();
    const QString ssidFmt = ssid.toHtmlEscaped();
    const QString ssidHex = QString::fromLatin1(ssid.toUtf8().toHex());

    /* Create XML profile */
    const QString xml = QString
    (
        "<?xml version=\"1.0\"?>"
        "<WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">"
            "<name>%1</name>"
            "<SSIDConfig>"
                "<SSID>"
                    "<hex>%2</hex>"
                    "<name>%1</name>"
                "</SSID>"
            "</SSIDConfig>"
            "<connectionType>ESS</connectionType>"
            "<connectionMode>manual</connectionMode>"
            "<autoSwitch>false</autoSwitch>"
            "<MSM>"
                "<security>"
                    "%3"
                "</security>"
            "</MSM>"
        "</WLANProfile>"
    ).arg(
        ssidFmt,
        ssidHex,
        blockSecurityToXml()
    );

    return xml;
}

/*!
 * \brief ProfileWinNative::blockSecurityToXml
 *
 * \note
 * https://learn.microsoft.com/fr-fr/windows/win32/nativewifi/wlan-profileschema-authencryption-security-element
 *
 * \return
 */
QString ProfileWinNative::blockSecurityToXml() const
{
    QString xmlSecurity;

    switch(m_network.getAuthAlgo())
    {
        case AuthAlgo::AUTH_ALGO_OPEN:{
            xmlSecurity = QString
            (
                "<authEncryption>"
                    "<authentication>open</authentication>"
                    "<encryption>none</encryption>"
                    "<useOneX>false</useOneX>"
                "</authEncryption>"
            );
        }break;

        case AuthAlgo::AUTH_ALGO_WPA2_PERSONAL:
        case AuthAlgo::AUTH_ALGO_WPA3_PERSONAL:{
            xmlSecurity = QString
            (
                "<authEncryption>"
                    "<authentication>%1</authentication>"
                    "<encryption>%2</encryption>"
                    "<useOneX>false</useOneX>"
                "</authEncryption>"
                "<sharedKey>"
                    "<keyType>passPhrase</keyType>"
                    "<protected>false</protected>"
                    "<keyMaterial>%3</keyMaterial>"
                "</sharedKey>"
            ).arg(
                fieldSecurityAuthToXml(),
                fieldSecurityEncryptionToXml(),
                m_passkey
            );
        }break;

        default: break;
    }

    return xmlSecurity;
}

QString ProfileWinNative::fieldSecurityAuthToXml() const
{
    switch(m_network.getAuthAlgo())
    {
        case AuthAlgo::AUTH_ALGO_OPEN:              return "open"; break;

        case AuthAlgo::AUTH_ALGO_WPA2_PERSONAL:     return "WPA2PSK"; break;
        case AuthAlgo::AUTH_ALGO_WPA2_ENTERPRISE:   return "WPA2"; break;

        case AuthAlgo::AUTH_ALGO_WPA3_PERSONAL:     return "WPA3SAE"; break;
        case AuthAlgo::AUTH_ALGO_WPA3_ENTERPRISE:   return "WPA3ENT"; break;

        default:                                    return "WPA2PSK"; break; // Use most common auth algo if unknown
    }
}

QString ProfileWinNative::fieldSecurityEncryptionToXml() const
{
    switch(m_network.getCipherAlgo())
    {
        case CipherAlgo::CIPHER_ALGO_TKIP:  return "TKIP"; break;
        case CipherAlgo::CIPHER_ALGO_AES:   return "AES"; break;
        case CipherAlgo::CIPHER_ALGO_GCMP:  return "GCMP256"; break;

        default:                            return "none"; break;
    }
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
