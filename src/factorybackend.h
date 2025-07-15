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

class Permissions;
class PermissionsPrivate;

/*****************************/
/* Class definitions         */
/*****************************/

class FactoryBackend
{

public:
    static std::unique_ptr<ManagerPrivate> createEngine(Manager *parent);
    static std::unique_ptr<PermissionsPrivate> createPermissions(Permissions *parent);

private:
    static void registerTypesQt();
};

/*****************************/
/* End namespaces            */
/*****************************/

} // namespace qwm

#endif // QWLANMAN_FACTORYBACKEND_H
