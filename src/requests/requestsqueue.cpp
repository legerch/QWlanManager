#include "requestsqueue.h"

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

RequestsQueue::RequestsQueue()
{
    /* Nothing to do */
}

void RequestsQueue::enqueue(const Request &request)
{
    const RequestType idType = request.getType();

    /* Verify duplicates */
    if(m_reqs.contains(idType)){
        m_queue.removeOne(idType);
    }

    /* Enqueue request */
    m_reqs.insert(idType, request);
    m_queue.enqueue(idType);
}

std::optional<Request> RequestsQueue::dequeue()
{
    /* Can we dequeue ? */
    if(isEmpty()){
        return std::nullopt;
    }

    /* Dequeue request */
    const RequestType idType = m_queue.takeFirst();
    const Request req = m_reqs.take(idType);

    return req;
}

bool RequestsQueue::isEmpty() const
{
    return m_queue.isEmpty();
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
