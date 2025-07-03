#include "factorybackend.h"

#include <QOperatingSystemVersion>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#   include <QtGlobal>
#else
#   include <QtSystemDetection>
#endif

#include "backend/mock/enginemock.h"        // IWYU pragma: keep
#include "backend/mock/permissionsmock.h"

#if defined(Q_OS_WINDOWS)
#   include "backend/winnative/enginewinnative.h"
#   if defined(QWLANMAN_HAVE_WINRT)
#       include "backend/winrt/permissionwinrt.h"
#   endif
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
#if defined(Q_OS_WINDOWS)
    qDebug("Use engine \"Windows Native\"");
    return std::make_unique<EngineWinNative>(parent);

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

#else
    qDebug("Use permissions \"Mock\"");
    return std::make_unique<PermissionsMock>(parent);
#endif
}

/*****************************/
/* End namespace             */
/*****************************/

} // namespace qwm

/*****************************/
/* End file                  */
/*****************************/
