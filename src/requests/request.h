#ifndef QWLANMAN_REQUESTS_REQUEST_H
#define QWLANMAN_REQUESTS_REQUEST_H

#include "qwlanmanager/datas/network.h"
#include "qwlanmanager/qwlantypes.h"

#include <QVariant>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class Request
{
private:
    Request(RequestType idReq, const QVariant &data = QVariant());

public:
    RequestType getType() const;
    const QVariant& getData() const;

private:
    RequestType m_type;
    QVariant m_data;

public:
    static Request createReqScan();
    static Request createReqConnect(const Network &net, const QString &password);
    static Request createReqDisconnect();
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_REQUESTS_REQUEST_H
