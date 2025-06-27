#ifndef QWLANMAN_DATAS_INTERFACEDATA_H
#define QWLANMAN_DATAS_INTERFACEDATA_H

#include "qwlanmanager/datas/network.h"
#include "qwlanmanager/qwlantypes.h"

#include <QMutex>
#include <QUuid>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

//TODO: implement cache on network list (some scan give 1 or 2 network insteads of all available)
//TODO: add a member allowing engine native platform to use custom type (maybe "void*" ?)
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
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_DATAS_INTERFACEDATA_H
