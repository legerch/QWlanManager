#ifndef QWLANMAN_QWLANHELPER_H
#define QWLANMAN_QWLANHELPER_H

#include <QFlags>
#include <QMetaEnum>
#include <QString>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm::Helper
{

/*****************************/
/* Classical methods         */
/*****************************/

int calcSignalSnr(int rssi, int noise);

int calcSignalPercent(int snr);
int calcSignalPercent(int rssi, int noise);

/*****************************/
/* Templated methods         */
/*****************************/

template<typename Flags>
QString flagsToString(Flags flags)
{
    /* Manage empty flag */
    if(flags == 0){
        return "0";
    }

    /* Search enum "ID" */
    const QMetaEnum metaEnum = QMetaEnum::fromType<Flags>();

    /* Build the string */
    const int nbValues = metaEnum.keyCount();

    QStringList result;
    for(int i = 0; i < nbValues; ++i){
        // Retrieve flag value
        const int value = metaEnum.value(i);
        if(value == 0){
            continue;
        }

        // Is this flag set ?
        if((flags & value) == value){
            result << QString::fromLatin1(metaEnum.key(i));
            flags &= ~value; // Remove treated flag
        }
    }

    return result.join('|');
}

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm::Helper

/*****************************/
/* Qt specific meta-system   */
/*****************************/

#endif // QWLANMAN_QWLANHELPER_H
