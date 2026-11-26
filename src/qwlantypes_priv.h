#ifndef QWLANMAN_QWLANTYPES_PRIV_H
#define QWLANMAN_QWLANTYPES_PRIV_H

#include <QHash>
#include <QObject>
#include <QString>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm::Internal
{
    Q_NAMESPACE

/*****************************/
/* Library private types     */
/*****************************/

    /*!
     * \brief Type of interface asynchrone request
     *
     * \sa requestTypeToString()
     */
    enum class RequestType
    {
        REQ_INVALID = 0,    /**< Invalid request */

        REQ_SCAN,           /**< Scan request */
        REQ_CONNECT,        /**< Connection request */
        REQ_DISCONNECT      /**< Disconnection request */
    };
    Q_ENUM_NS(RequestType);

    QString requestTypeToString(RequestType idReq);

/*****************************/
/* Qt related methods        */
/*****************************/

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)

inline uint qHash(qwm::Internal::RequestType key, uint seed = 0)
{
    return ::qHash(static_cast<std::underlying_type<qwm::Internal::RequestType>::type>(key), seed);
}

#endif

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm::Internal

#endif // QWLANMAN_QWLANTYPES_PRIV_H
