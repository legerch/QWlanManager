#ifndef QWLANMAN_DATAS_NETWORKDATA_H
#define QWLANMAN_DATAS_NETWORKDATA_H

#include <QMutex>
#include <QString>

#include <any>

#include "cache/cacheinfo.h"
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
    uint m_signalQuality;

    CacheInfo m_cache;

    std::any m_dataEngine;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_NETWORKDATA_H
