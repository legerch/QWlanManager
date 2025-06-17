#include "qwlanmanager/interface.h"

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
/*      Public Class         */
/*****************************/

Interface::Interface() = default;

const QUuid& Interface::getUid() const
{
    return m_uid;
}

const QString& Interface::getHwAddress() const
{
    return m_hwAddress;
}

const QString& Interface::getName() const
{
    return m_name;
}

const QString& Interface::getFriendlyName() const
{
    return m_friendlyName;
}

const QString& Interface::getDescription() const
{
    return m_description;
}

bool Interface::isUp() const
{
    return m_isUp;
}

bool Interface::operator==(const Interface &other) const
{
    return m_uid == other.m_uid;
}

bool Interface::operator!=(const Interface &other) const
{
    return !(*this == other);
}

bool Interface::operator<(const Interface &other) const
{
    return m_uid < other.m_uid;
}

bool Interface::operator>(const Interface &other) const
{
    return m_uid > other.m_uid;
}

bool Interface::operator<=(const Interface &other) const
{
    return !(*this > other);
}

bool Interface::operator>=(const Interface &other) const
{
    return !(*this < other);
}

/*****************************/
/* Qt custom related methods */
/*****************************/

size_t qHash(const Interface &key, uint seed)
{
    return qHash(key.getUid(), seed);
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
