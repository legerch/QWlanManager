#ifndef QWLANMAN_BACKEND_COREWLAN_ENGINECOREWLAN_H
#define QWLANMAN_BACKEND_COREWLAN_ENGINECOREWLAN_H

#include "manager_priv.h"

#include <QThread>

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/* WorkerCoreWlan            */
/*****************************/

class WorkerCoreWlan : public QObject
{
    Q_OBJECT

public:
    explicit WorkerCoreWlan(QObject *parent = nullptr);
    ~WorkerCoreWlan();

signals:
    void sScanDone(const Interface &interface, WlanError result);

public slots:
    void performScan(const Interface &interface);
};

/*****************************/
/* Class definitions         */
/* EngineCoreWlan            */
/*****************************/

class EngineCoreWlan final : public ManagerPrivate
{
public:
    explicit EngineCoreWlan(Manager *parent);
    ~EngineCoreWlan();

public:
    void initialize() override;
    void terminate() override;

public:
    void interfaceListRefresh() override;

protected:
    void interfaceScanNetworksAsync(Interface interface) override;
    void interfaceConnectAsync(Interface interface, Network network, const QString &password) override;
    void interfaceDisconnectAsync(Interface interface) override;
    void interfaceForgetAsync(Interface interface, Network network) override;

private:
    void registerWorkerEvents();

private:
    QThread m_thread;
    WorkerCoreWlan *m_worker = nullptr;

private:
    static const QUuid NS_UID;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_COREWLAN_ENGINECOREWLAN_H
