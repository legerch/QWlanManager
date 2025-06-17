#ifndef QWLANMAN_INTERFACE_H
#define QWLANMAN_INTERFACE_H

#include <QObject>
#include <QUuid>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

//TODO: when retrieveing interfaces, filter "virtual"

class Interface
{
    Q_GADGET

    Q_PROPERTY(QUuid uid READ getUid)
    Q_PROPERTY(QString hwAddress READ getHwAddress)
    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(QString friendlyName READ getFriendlyName)
    Q_PROPERTY(QString description READ getDescription)
    Q_PROPERTY(bool isUp READ isUp)

public:
    Interface();

public:
    const QUuid& getUid() const;
    const QString& getHwAddress() const;
    const QString& getName() const;
    const QString& getFriendlyName() const;
    const QString& getDescription() const;
    bool isUp() const;

public:
    bool operator==(const Interface &other) const;
    bool operator!=(const Interface &other) const;
    bool operator<(const Interface &other) const;
    bool operator>(const Interface &other) const;
    bool operator<=(const Interface &other) const;
    bool operator>=(const Interface &other) const;

private:
    QUuid m_uid;
    QString m_hwAddress;
    QString m_name;
    QString m_friendlyName;
    QString m_description;
    bool m_isUp;
};

/*****************************/
/* Qt custom related methods */
/*****************************/
size_t qHash(const Interface &key, uint seed = 0);

/*****************************/
/* Alias for related types   */
/*****************************/

using ListInterfaces = QList<Interface>;
using MapInterfaces = QHash<QString, Interface>; /**< Key is interface UID */

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_INTERFACE_H
