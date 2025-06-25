#ifndef QWLANMAN_BACKEND_WINNATIVE_PROFILEWINNATIVE_H
#define QWLANMAN_BACKEND_WINNATIVE_PROFILEWINNATIVE_H

#include "qwlanmanager/datas/network.h"

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class ProfileWinNative
{
public:
    ProfileWinNative(const Network &network, const QString &passkey);

public:
    const QString& getName() const;

    QString toXmlFormat() const;

private:
    QString blockSecurityToXml() const;

    QString fieldSecurityAuthToXml() const;
    QString fieldSecurityEncryptionToXml() const;

private:
    QString m_name;
    Network m_network;
    QString m_passkey;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_WINNATIVE_PROFILEWINNATIVE_H
