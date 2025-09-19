#ifndef QWLANMAN_BACKEND_NM_TYPESNM_H
#define QWLANMAN_BACKEND_NM_TYPESNM_H

#include <QObject>
#include <QUuid>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm::NmBus
{
    Q_NAMESPACE

    bool isWifiDevice(int idType);

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm::NmBus

#endif // QWLANMAN_BACKEND_NM_TYPESNM_H
