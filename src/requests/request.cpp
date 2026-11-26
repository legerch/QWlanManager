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

Request::Request()
    : Request(Internal::RequestType::REQ_INVALID)
{
    /* Nothing to do, use constructor delegation */
}

Request::Request(Internal::RequestType idReq, const QVariant &data)
    : m_type(idReq), m_data(data)
{
    /* Nothing to do */
}

bool Request::isValid() const
{
    return m_type != Internal::RequestType::REQ_INVALID;
}

Internal::RequestType Request::getType() const
{
    return m_type;
}

const QVariant &Request::getData() const
{
    return m_data;
}

Request Request::createReqScan()
{
    return Request(Internal::RequestType::REQ_SCAN);
}

Request Request::createReqConnect(const Network &net, const QString &password)
{
    /* Create associated datas */
    QVariantMap map;
    map["network"] = QVariant::fromValue(net);
    map["password"] = password;

    /* Create request */
    return Request(Internal::RequestType::REQ_CONNECT, map);
}

Request Request::createReqDisconnect()
{
    return Request(Internal::RequestType::REQ_DISCONNECT);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
