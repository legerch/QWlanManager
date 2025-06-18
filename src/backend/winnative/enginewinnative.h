#ifndef QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H
#define QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H

#include "manager_priv.h"

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class EngineWinNative final : public ManagerPrivate
{
public:
    explicit EngineWinNative(Manager *parent);
    ~EngineWinNative();

public:
    void initialize() override;
    void terminate() override;

public:
    void refreshInterfaces() override;

private:
    bool apiOpen();
    void apiClose();

    bool interfaceIsVirtual(const QString &description);

private:
    Qt::HANDLE m_handle;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_WINNATIVE_ENGINEWINNATIVE_H
