#ifndef QWLANMAN_BACKEND_ENGINEMOCK_H
#define QWLANMAN_BACKEND_ENGINEMOCK_H

#include "manager_priv.h"

/*****************************/
/* Namespace instructions    */
/*****************************/

namespace qwm
{

/*****************************/
/* Class definitions         */
/*****************************/

class EngineMock final : public ManagerPrivate
{
public:
    explicit EngineMock(Manager *parent);
    ~EngineMock();

public:
    void initialize() override;
    void terminate() override;
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_BACKEND_ENGINEMOCK_H
