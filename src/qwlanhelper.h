#ifndef QWLANMAN_QWLANHELPER_H
#define QWLANMAN_QWLANHELPER_H

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm::Helper
{

int calcSignalSnr(int rssi, int noise);

int calcSignalPercent(int snr);
int calcSignalPercent(int rssi, int noise);

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm::Helper

/*****************************/
/* Qt specific meta-system   */
/*****************************/

#endif // QWLANMAN_QWLANHELPER_H
