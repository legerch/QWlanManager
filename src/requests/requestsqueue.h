#ifndef QWLANMAN_REQUESTS_REQUESTSQUEUE_H
#define QWLANMAN_REQUESTS_REQUESTSQUEUE_H

#include "request.h"

#include <QQueue>

#include <optional>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class RequestsQueue
{
public:
    RequestsQueue();

public:
    void enqueue(const Request &request);
    std::optional<Request> dequeue();

public:
    bool isEmpty() const;

private:
    QHash<Internal::RequestType, Request> m_reqs;
    QQueue<Internal::RequestType> m_queue;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_REQUESTS_REQUESTSQUEUE_H
