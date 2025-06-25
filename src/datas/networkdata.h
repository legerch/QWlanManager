#ifndef QWLANMAN_DATAS_NETWORKDATA_H
#define QWLANMAN_DATAS_NETWORKDATA_H

#include <QMutex>
#include <QString>

#include "qwlanmanager/qwlantypes.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class NetworkData
{

public:
    NetworkData();

public:
    bool operator==(const NetworkData &other) const;
    bool operator!=(const NetworkData &other) const;

public:
    mutable QMutex m_mutex;

    QString m_ssid;
    QString m_profileName;
    AuthAlgo m_idAuth;
    CipherAlgo m_idCipher;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_NETWORKDATA_H
