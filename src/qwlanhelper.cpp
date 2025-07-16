#include "qwlanhelper.h"

#include <QtNumeric>

/*****************************/
/* Namespace documentations  */
/*****************************/

/*!
 * \namespace qwm::Helper
 * \brief Namespace used to store
 * internal helper methods
 */

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm::Helper
{

/*!
 * \brief Calculate signal quality using
 * <b>Signal to Noise Ratio (SNR)</b>.
 *
 * \param[in] rssi
 * Value of the <em>Received Signal Strength Indicator</em>. \n
 * It measures how strong the signal from the access point is. \n
 * Reported in \b dBm (decibels relative to <em>1 milliwatt</em>),
 * a negative number, e.g., <tt>-40 dBm</tt> (strong) to <tt>-90 dBm</tt> (weak).
 * \param[in] noise
 * Value of the <em>ambient noise level</em> detected by the interface. \n
 * Reported in \b dBm, , typically in the range of <tt>-90 dBm</tt> to <tt>-100 dBm</tt>. \n
 * This value represents how "noisy" the RF environment is (other devices, interference, etc...)
 *
 * \note
 * Some ressources:
 * - https://www.netally.com/tech-tips/understand-noise-with-wi-fi-snr/
 * - https://community.tp-link.com/en/business/forum/topic/274452
 *
 * \return
 * Return \b SNR value
 *
 * \sa calcSignalPercent()
 */
int calcSignalSnr(int rssi, int noise)
{
    return rssi - noise;
}

/*!
 * \brief Calculate signal quality percent
 * from \em SNR value.
 * \details
 * Please see \c calcSignalSnr() for more details.
 *
 * \param[in] snr
 * Value of <b>Signal to Noise Ratio (SNR)</b>.
 *
 * \return
 * Return signal quality in percent.
 *
 * \sa calcSignalSnr()
 */
int calcSignalPercent(int snr)
{
    /* Define ranges */
    constexpr int minSnr = 0;
    constexpr int maxSnr = 40;
    constexpr int rangeSnr = maxSnr - minSnr;

    /* Convert to percentage */
    if(snr <= minSnr){
        return 0;
    }

    if(snr >= maxSnr){
        return 100;
    }

    double percent = (100.0 * (snr - minSnr) / rangeSnr);
    return qRound(percent);
}

/*!
 * \brief Calculate signal quality percent.
 * \details
 * Please see \c calcSignalSnr() for more details.
 *
 * \param[in] rssi
 * Value of the <em>Received Signal Strength Indicator</em>. \n
 * It measures how strong the signal from the access point is. \n
 * Reported in \b dBm (decibels relative to <em>1 milliwatt</em>),
 * a negative number, e.g., <tt>-40 dBm</tt> (strong) to <tt>-90 dBm</tt> (weak).
 * \param[in] noise
 * Value of the <em>ambient noise level</em> detected by the interface. \n
 * Reported in \b dBm, , typically in the range of <tt>-90 dBm</tt> to <tt>-100 dBm</tt>. \n
 * This value represents how "noisy" the RF environment is (other devices, interference, etc...)
 *
 * \return
 * Return signal quality in percent.
 *
 * \sa calcSignalSnr()
 */
int calcSignalPercent(int rssi, int noise)
{
    const int snr = calcSignalSnr(rssi, noise);
    return calcSignalPercent(snr);
}

/*****************************/
/* Functions implementation  */
/*****************************/

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm::Helper

/*****************************/
/* End file                  */
/*****************************/
