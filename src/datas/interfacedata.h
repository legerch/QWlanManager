#ifndef QWLANMAN_DATAS_INTERFACEDATA_H
#define QWLANMAN_DATAS_INTERFACEDATA_H

#include "qwlanmanager/cache/cachepolicy.h"
#include "qwlanmanager/datas/network.h"
#include "qwlanmanager/qwlantypes.h"

#include "requests/requestsqueue.h"

#include <QMutex>
#include <QUuid>

#include <any>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class InterfaceData
{

public:
    InterfaceData();

public:
    bool operator==(const InterfaceData &other) const;
    bool operator!=(const InterfaceData &other) const;

public:
    mutable QMutex m_mutex;

    IfaceState m_state;
    QUuid m_uid;
    QString m_hwAddress;
    QString m_name;
    QString m_description;

    MapNetworks m_mapNets;
    QString m_connectedSsid;

    CachePolicy m_cachePolicy;

    RequestsQueue m_reqsQueue;
    std::any m_dataEngine;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_INTERFACEDATA_H
