#ifndef QWLANMAN_FACTORYBACKEND_H
#define QWLANMAN_FACTORYBACKEND_H

#include <memory>

/*****************************/
/* Namespace instructions    */
/*****************************/
namespace qwm
{

/*****************************/
/* Forwards classes          */
/*****************************/

class Manager;
class ManagerPrivate;

/*****************************/
/* Class definitions         */
/*****************************/

class FactoryBackend
{

public:
    static std::unique_ptr<ManagerPrivate> createBackend(Manager *parent);
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_FACTORYBACKEND_H
