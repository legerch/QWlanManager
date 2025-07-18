#include "factorybackend.h"

#include <QOperatingSystemVersion>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#   include <QtGlobal>
#else
#   include <QtSystemDetection>
#endif

#include "backend/mock/enginemock.h"        // IWYU pragma: keep
#include "backend/mock/permissionsmock.h"   // IWYU pragma: keep

#if defined(Q_OS_WINDOWS)
#   include "backend/winnative/enginewinnative.h"
#   if defined(QWLANMAN_HAVE_WINRT)
#       include "backend/winrt/permissionwinrt.h"
#   endif
#elif defined(Q_OS_MACOS)
#   include "backend/corewlan/enginecorewlan.h"
#   include "backend/corewlan/permissioncorewlan.h"
#endif


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
/*****************************/

std::unique_ptr<ManagerPrivate> FactoryBackend::createEngine(Manager *parent)
{
    /* Register custom types to Qt */
    registerTypesQt();

    /* Create engine backend */
#if defined(Q_OS_WINDOWS)
    qDebug("Use engine \"Windows Native\"");
    return std::make_unique<EngineWinNative>(parent);

#elif defined(Q_OS_MACOS)
    qDebug("Use engine \"CoreWlan\"");
    return std::make_unique<EngineCoreWlan>(parent);

#else
    qDebug("Use engine \"Mock\"");
    return std::make_unique<EngineMock>(parent);
#endif
}

std::unique_ptr<PermissionsPrivate> FactoryBackend::createPermissions(Permissions *parent)
{
#if defined(Q_OS_WINDOWS) && defined(QWLANMAN_HAVE_WINRT)

    /* Verify if current Windows OS version has minimum WinRT version */
#if QT_VERSION < QT_VERSION_CHECK(6, 3, 0)
    constexpr QOperatingSystemVersion win11{QOperatingSystemVersion::Windows, 10, 0, 22000};
#else
    constexpr QOperatingSystemVersion win11 = QOperatingSystemVersion::Windows11;
#endif
    const QOperatingSystemVersion sysVersion = QOperatingSystemVersion::current();
    if(sysVersion >= win11){
        qDebug("Use permission \"Windows WinRT\"");
        return std::make_unique<PermissionWinRt>(parent);
    }

    /* WinRT not supported */
    qDebug("Use permissions \"Mock\"");
    return std::make_unique<PermissionsMock>(parent);

#elif defined(Q_OS_MACOS)
    qDebug("Use permissions \"CoreWlan\"");
    return std::make_unique<PermissionCoreWlan>(parent);

#else
    qDebug("Use permissions \"Mock\"");
    return std::make_unique<PermissionsMock>(parent);
#endif
}

void FactoryBackend::registerTypesQt()
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static bool isInit = false;
    if(!isInit){
        isInit = true;

        qRegisterMetaType<qwm::ListInterfaces>("ListInterfaces");
        qRegisterMetaType<qwm::MapInterfaces>("MapInterfaces");

        qRegisterMetaType<qwm::ListNetworks>("ListNetworks");
        qRegisterMetaType<qwm::MapNetworks>("MapNetworks");
    }
#endif
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
