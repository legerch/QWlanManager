[QWlanManager][repo-home] is a Qt library allowing to manage WLAN interfaces and network easily without worrying about OS dependencies.

> [!TIP]
> Latest development/pull requests will be committed into `main` branch.  
> Each stable release have their dedicated branch:
> - `1.0.x`: branch `dev/1.0`
> - `1.1.x`: branch `dev/1.1`
> - etc...

**Table of contents :**
- [1. Library details](#1-library-details)
  - [1.1. Features](#11-features)
  - [1.2. Supported platforms](#12-supported-platforms)
    - [1.2.1. Status](#121-status)
    - [1.2.2. Specific behaviours](#122-specific-behaviours)
      - [1.2.2.1. MacOS](#1221-macos)
- [2. Requirements](#2-requirements)
  - [2.1. C++ Standards](#21-c-standards)
  - [2.2. Dependencies](#22-dependencies)
- [3. How to build](#3-how-to-build)
  - [3.1. CMake usage](#31-cmake-usage)
  - [3.2. CMake options](#32-cmake-options)
- [4. How to use](#4-how-to-use)
  - [4.1. Usage](#41-usage)
  - [4.2. Library version](#42-library-version)
    - [4.2.1. Compatibility](#421-compatibility)
    - [4.2.2. Compilation time](#422-compilation-time)
    - [4.2.3. Runtime](#423-runtime)
- [5. Documentation](#5-documentation)
- [6. License](#6-license)

# 1. Library details
## 1.1. Features

This cross-platform library allow to control interfaces operations (_perform a scan, connect to a network, etc..._) and manage all related events through the class `qwm::Manager`. Main features of the library are:
- Asynchronous operations: no blocking operations, all event-driven
- Catch "external" wifi events (wifi status changed through OS UI, from other apps, etc...)
- Interfaces added or removed are detected
- UTF-8 networks are supported
- Datas classes are **QML** compatible
- Implicit shared datas (via `shared_ptr`) reducing number of copies (passing by _value_ is cheap)
- Library is thread-safe, no need for external synchronisation management
- Allow to manage permissions of the _Operating System:_ some OSes required **Wlan related authorizations**. Class `qwm::Permissions` will help to manage those.
- Allow to enable/disable a _cache feature:_ Some OSes can return a partial scan, it doesn't fail but will be partial (_due to hardware error, wrong timing, etc..._). Having a cache can help to smooth the list of network management
- Allow to enable/disable a _request feature:_ Performing interface operations in the same time (like a connection during a scan, a scan during connection, etc...) can trigger an unexpected behaviour. Request feature allow to queue those operations.

What the library **doesn't** support (yet!):
- WPA2/WPA3 enterprise networks
- _Hidden networks_ are ignored
- _Peer-to-peer/ad-hoc networks_ are ignored

## 1.2. Supported platforms
### 1.2.1. Status
One OS can have multiple backend or differ according to the OS version used, this table will reflect those differences:

| OS | Backend engine | Permissions backend | Status | Comments |
|:-:|:-:|:-:|:-:|:-:|
| Windows 7/8 | [WlanAPI][windows-wlanapi]<br>[IpHlpAPI][windows-iphlpapi] | 🚫 | ✅ | Custom _CMake options_ [`QWLANMANAGER_WINDOWS_COMPAT_PREWIN10`][anchor-cmake-opts] required<br>**Qt5.15** will be needed for those platforms (Qt6 isn't compatible with those) |
| Windows 10/11 | [WlanAPI][windows-wlanapi]<br>[IpHlpAPI][windows-iphlpapi] | [WinRT][windows-runtime]<br>(via [AppCapabilityAccess][windows-runtime-perms]) | ✅ | Package `cppwinrt` required for _permissions backend_ |
| Windows 11 | [WinRT][windows-runtime]<br>(via [WifiAdapter][windows-runtime-perms]) | [WinRT][windows-runtime]<br>(via [AppCapabilityAccess][windows-runtime-perms]) | 🕚 | Package `cppwinrt` required for _wifi and permissions backend_ |
| MacOS | [CoreWlan][mac-corewlan] | [CoreLocation][mac-corelocation] | ✅ | Please refer to section [specific behaviour - MacOS][anchor-spec-macos] |
| Linux | NetworkManager | ❓ | 🕚 | / |

> [!NOTE]
> Legends:
> - 🚫: No support on native API
> - 🕚: Planned
> - 📝: In progress
> - ✅: Complete support

### 1.2.2. Specific behaviours
#### 1.2.2.1. MacOS

Under **MacOS**, location permissions key [`NSLocationUsageDescription`][mac-perm-location] must be declared inside your [`Info.plist`][mac-plist] application file in order to perform a network scan:
```xml
<key>NSLocationUsageDescription</key>
<string>Used to scan nearby Wi-Fi networks</string>
```

# 2. Requirements
## 2.1. C++ Standards

This library requires at least **C++ 17** standard

## 2.2. Dependencies

Below, list of required dependencies:

| Dependencies | VCPKG package | Comments |
|:-:|:-:|:-:|
| [Qt][qt-official] | / | Compatible with **Qt6.x**<br>Compatible with **Qt5.15.x** |
| Native APIs | / | Please refer to section [supported platform][anchor-platforms] for more details |

> [!NOTE]
> Dependency manager [VCPKG][vcpkg-tutorial] is not mandatory, this is only a note to be able to list needed packages

# 3. How to build
## 3.1. CMake usage

This library can be use as an _embedded library_ in a subdirectory of your project (like a _git submodule_ for example) :
1. In the **root** CMakeLists, add instructions :
```cmake
add_subdirectory(qwlanmanager) # Or if library is put in a folder "dependencies": add_subdirectory(dependencies/qwlanmanager)
```

2. In the **application/library** CMakeLists, add instructions :
```cmake
# Link needed libraries
target_link_libraries(${PROJECT_NAME} PRIVATE qwlanmanager)
```

## 3.2. CMake options

This library provide some **CMake** build options:
- `QWLANMANAGER_WINDOWS_COMPAT_PREWIN10` (default: `OFF`): Use this option to enable compatibility with Windows version earlier than _Windows 10_. Enabling this option will disable some features in order to keep compatibility, so better to disable it for Windows 10 and newer. Internally, this is due to the missing support of _C++/WinRT_ library, impacted features will be :
  - `qwm:Permissions`: no support for permissions control, each permissions method will use a _mock_ version, allowing users of the library to not have to make distinction code, permissions will simply return an _unknown status_.

# 4. How to use
## 4.1. Usage

Please refer to `qwm::Manager` class documentation for more details.

## 4.2. Library version
### 4.2.1. Compatibility

This library use the [PImpl Idiom][pimpl-doc-cpp] in order to preserve _ABI compatibility_ (_Qt wiki_ also have a [great tutorial on the PImpl idiom][pimpl-doc-qt]).  
So only **major** release (this project use the [semantic versioning][semver-home]) _should_ break the ABI.

### 4.2.2. Compilation time

In order to easily check at compilation time library version (to manage compatibility between multiple versions for example), macro `QWLANMAN_VERSION_ENCODE` (defined inside _qwlanman_global.h_ file) can be used:
```cpp
#if QWLANMAN_VERSION >= QWLANMAN_VERSION_ENCODE(2,0,0)
    // Do stuff for version 2.0.0 or higher
#else
    // Do stuff for earlier versions
#endif
```

### 4.2.3. Runtime

Since library header used during final application build could differ from the actual library version, it is recommended to use the method:
```cpp
#include "qwlanmanager/qwlantypes.h"

const QVersionNumber libSemver = qwm::getLibraryVersion();
```

# 5. Documentation

All classes/methods has been documented with [Doxygen][doxygen-official] utility and automatically generated at [online website documentation][repo-doc-web].

> [!NOTE]
> This repository contains two kinds of documentation:
> - **Public API:** Available via [online website documentation][repo-doc-web] or locally via Doxyfile `docs/fragments/Doxyfile-public-api.in`
> - **Internal:** Available locally only via `docs/fragments/Doxyfile-internal.in`

To generate documentation locally, we can use:
```shell
doxygen ./docs/fragments/Doxyfile-name
```
> [!TIP]
> You can also load the _Doxyfile_ into _Doxywizard_ (Doxygen GUI) and run generation.

# 6. License

This library is licensed under [MIT license][repo-license-url].

<!-- Anchor of this page -->
[anchor-platforms]: #12-supported-platforms
[anchor-cmake-opts]: #32-cmake-options
[anchor-spec-macos]: #1221-macos

<!-- Links of this repository -->
[repo-home]: https://github.com/legerch/QWlanManager
[repo-doc-web]: https://legerch.github.io/QWlanManager/
[repo-license]: LICENSE
[repo-license-url]: https://github.com/legerch/QWlanManager/LICENSE

<!-- External links -->
[doxygen-official]: https://www.doxygen.nl/index.html
[example-doc-web]: https://www.google.com/

[qt-official]: https://www.qt.io/

[vcpkg-tutorial]: https://github.com/legerch/develop-memo/tree/master/Toolchains/Build%20systems/VCPKG

[semver-home]: https://semver.org
[pimpl-doc-cpp]: https://en.cppreference.com/w/cpp/language/pimpl
[pimpl-doc-qt]: https://wiki.qt.io/D-Pointer

[mac-corelocation]: https://developer.apple.com/documentation/corelocation?language=objc
[mac-corewlan]: https://developer.apple.com/documentation/corewlan
[mac-plist]: https://developer.apple.com/documentation/bundleresources/information-property-list?language=objc
[mac-perm-location]: https://developer.apple.com/documentation/bundleresources/information-property-list/nslocationusagedescription?language=objc

[windows-wlanapi]: https://docs.microsoft.com/en-us/windows/win32/api/wlanapi/
[windows-iphlpapi]: https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/
[windows-runtime]: https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt
[windows-runtime-perms]: https://learn.microsoft.com/en-us/uwp/api/windows.security.authorization.appcapabilityaccess?view=winrt-26100
[windows-runtime-wifi]: https://learn.microsoft.com/fr-fr/uwp/api/windows.devices.wifi.wifiadapter?view=winrt-26100