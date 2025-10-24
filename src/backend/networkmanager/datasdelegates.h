#ifndef QWLANMAN_BACKEND_NM_DATASDELEGATES_H
#define QWLANMAN_BACKEND_NM_DATASDELEGATES_H

#include "qwlanmanager/datas/interface.h"

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/* DelegateInterface         */
/*****************************/

class DelegateInterface : public QObject
{
    Q_OBJECT

public:
    explicit DelegateInterface(Interface *parent, const QString &devicePath);
    ~DelegateInterface();

private slots:
    void handlePropertiesChanged(const QString &arg1, const QVariantMap &propsChanged, const QStringList &propsInvalidated);

private:
    void eventRegister();
    void eventUnregister();

private:
    Interface *m_refParent = nullptr;
    QString m_devPath;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_NM_DATASDELEGATES_H
