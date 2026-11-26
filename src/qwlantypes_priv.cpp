#include "qwlantypes_priv.h"

/*****************************/
/* Namespace documentations  */
/*****************************/

/*!
 * \namespace qwm::Internal
 * \brief Namespace used to store library
 * internal types (not part of public API).
 */

/*****************************/
/* Signals documentations    */
/*****************************/

/*****************************/
/* Start namespace           */
/*****************************/

namespace qwm::Internal
{

/*****************************/
/* Functions implementation  */
/*****************************/

/*!
 * \brief Use to convert request type to string.
 *
 * \param[in] idReq
 * Request ID to convert. \n
 * If unknown or invalid, string "invalid" will be returned.
 *
 * \return
 * Returns string equivalent.
 */
QString requestTypeToString(RequestType idReq)
{
    static const QHash<RequestType, QString> MAP_STR_REQS =
    {
        {RequestType::REQ_SCAN, "scan"},
        {RequestType::REQ_CONNECT, "connect"},
        {RequestType::REQ_DISCONNECT, "disconnect"},
    };

    return MAP_STR_REQS.value(idReq, "invalid");
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm::Internal

/*****************************/
/* End file                  */
/*****************************/
