#ifndef QWLANMAN_QWLANTYPES_H
#define QWLANMAN_QWLANTYPES_H

#include <QObject>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{
    Q_NAMESPACE

enum class WlanError
{
    WERR_NO_ERROR = 0
};
Q_ENUM_NS(WlanError)

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_QWLANTYPES_H
