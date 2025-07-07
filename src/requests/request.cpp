#include "request.h"

/*****************************/
/* Class documentations      */
/*****************************/

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm
{

/*****************************/
/* Functions implementation  */
/*****************************/

Request::Request(RequestType idReq, const QVariant &data)
    : m_type(idReq), m_data(data)
{
    /* Nothing to do */
}

RequestType Request::getType() const
{
    return m_type;
}

const QVariant &Request::getData() const
{
    return m_data;
}

Request Request::createReqScan()
{
    return Request(RequestType::REQ_SCAN);
}

Request Request::createReqConnect(const Network &net, const QString &password)
{
    /* Create associated datas */
    QVariantMap map;
    map["network"] = QVariant::fromValue(net);
    map["password"] = password;

    /* Create request */
    return Request(RequestType::REQ_CONNECT, map);
}

Request Request::createReqDisconnect()
{
    return Request(RequestType::REQ_DISCONNECT);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
