cmake_minimum_required(VERSION 3.20)

# Set project properties
set(PRJ_COMPANY_ID legerch)
set(PRJ_COMPANY_NAME "Leger Charlie")
set(PRJ_ID qwlanmanager)
set(PRJ_READABLE_NAME "QWlanManager")
set(PRJ_VERSION_SEMANTIC 0.0.1)
set(PRJ_DESCRIPTION "Allow to manage WLAN interfaces and network easily without worrying about OS dependencies.")
set(PRJ_LICENSE "MIT")
set(PRJ_COPYRIGHT "Copyright (c) 2025 ${PRJ_COMPANY_NAME} (${PRJ_LICENSE} License)")
set(PRJ_HOMEPAGE_URL "https://github.com/legerch/QWlanManager")

set(PROJECT_NAME ${PRJ_ID})
set(PROJECT_VERSION_CPP_MIN 17)

# Set project configuration
## Generic properties
project(${PROJECT_NAME}
    LANGUAGES       CXX
    VERSION         "${PRJ_VERSION_SEMANTIC}"
    DESCRIPTION     ${PRJ_DESCRIPTION}
    HOMEPAGE_URL    ${PRJ_HOMEPAGE_URL}
)

## Qt specific properties
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Define project options
# Example: option(QWLANMANAGER_BUILD_TESTS "Use to enable/disable build of unit-tests." ON)
option(QWLANMANAGER_WINDOWS_COMPAT_PREWIN10 "Support Windows versions older than Windows 10" OFF)

# Manage global compiler options
## For MSVC: force to read source code as UTF-8 file (already default behaviour on GCC/Clang)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# Set C++ standard to use
if(DEFINED CMAKE_CXX_STANDARD)
    if(${CMAKE_CXX_STANDARD} LESS ${PROJECT_VERSION_CPP_MIN})
        message(FATAL_ERROR "Project ${PROJECT_NAME} require at least C++ standard ${PROJECT_VERSION_CPP_MIN}")
    endif()
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Project \"${PROJECT_NAME}\" compiled with C++ standard ${CMAKE_CXX_STANDARD}")

# Set needed packages
## Qt packages
find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui REQUIRED)

## Other library packages
### Example: find_package(nlohmann_json 3.11.3 REQUIRED)

# Defines useful path variables for easier CMake configuration
set(PROJECT_DIR_PUBLIC_HEADERS_ROOT "${PROJECT_SOURCE_DIR}/include")
set(PROJECT_DIR_PUBLIC_HEADERS      "${PROJECT_DIR_PUBLIC_HEADERS_ROOT}/${PROJECT_NAME}")
set(PROJECT_DIR_PRIV_SOURCES        "${PROJECT_SOURCE_DIR}/src")

set(PROJECT_FILE_GLOBAL             "${PROJECT_DIR_PUBLIC_HEADERS}/qwlanman_global.h")

set(PROJECT_FILE_OS_METADATA_WINDOWS "${PROJECT_SOURCE_DIR}/metadata/windows/infos.rc")

# Manage library files
## Configure file project - File containing macros that can be used in project
configure_file("${PROJECT_FILE_GLOBAL}.in" "${PROJECT_FILE_GLOBAL}")
configure_file("${PROJECT_FILE_OS_METADATA_WINDOWS}.in" "${PROJECT_FILE_OS_METADATA_WINDOWS}")

## Set library compilation definitions
set(PROJECT_DEFS_PRIV
    "QWLANMAN_LIBRARY_BUILD"
)

set(PROJECT_DEFS_PUB

)

## Set library linking
### Linking dirs
set(PROJECT_LINKING_DIR

)

### Linking libs
set(PROJECT_LINKING_LIB
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
)

## Set library files
### Public interface
set(PROJECT_HEADERS_PUBLIC
    manager.h
    permissions.h
    qwlantypes.h

    cache/cachepolicy.h

    datas/interface.h
    datas/network.h
)

### Private
set(PROJECT_HEADERS_PRIVATE
    factorybackend.h
    manager_priv.h
    permissions_priv.h
    qwlanhelper.h

    backend/mock/enginemock.h
    backend/mock/permissionsmock.h

    cache/cacheinfo.h
    cache/cachepolicy_priv.h

    datas/interfacedata.h
    datas/interfacemutator.h
    datas/networkdata.h
    datas/networkmutator.h

    requests/request.h
    requests/requestsqueue.h
)

set(PROJECT_SOURCES
    factorybackend.cpp
    manager.cpp
    manager_priv.cpp
    permissions.cpp
    permissions_priv.cpp
    qwlanhelper.cpp
    qwlantypes.cpp

    backend/mock/enginemock.cpp
    backend/mock/permissionsmock.cpp

    cache/cacheinfo.cpp
    cache/cachepolicy.cpp
    cache/cachepolicy_priv.cpp

    datas/interface.cpp
    datas/interfacedata.cpp
    datas/interfacemutator.cpp
    datas/network.cpp
    datas/networkdata.cpp
    datas/networkmutator.cpp

    requests/request.cpp
    requests/requestsqueue.cpp
)

set(PROJECT_RESSOURCES
    # No ressources to set
)

# Manage platform differences
## Refer to: https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(PROJECT_DEFS_PUB ${PROJECT_DEFS_PUB}
        "NOMINMAX"  # Needed because Windows define macros "min/max", better to not defined those and use "std::min/std::max" instead
        "WIN32_LEAN_AND_MEAN=1"
    )

    set(PROJECT_RESSOURCES ${PROJECT_RESSOURCES}
        ${PROJECT_FILE_OS_METADATA_WINDOWS}
    )

    set(PROJECT_HEADERS_PRIVATE ${PROJECT_HEADERS_PRIVATE}
        backend/winnative/enginewinnative.h
        backend/winnative/profilewinnative.h
        backend/winnative/typeswinnative.h
    )

    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        backend/winnative/enginewinnative.cpp
        backend/winnative/profilewinnative.cpp
        backend/winnative/typeswinnative.cpp
    )

    set(PROJECT_LINKING_LIB ${PROJECT_LINKING_LIB}
        Wlanapi             # WlanAPI
        Iphlpapi            # IP helper tools
    )

    if(NOT QWLANMANAGER_WINDOWS_COMPAT_PREWIN10)
        set(PROJECT_DEFS_PRIV ${PROJECT_DEFS_PRIV}
            "QWLANMAN_HAVE_WINRT"
        )

        set(PROJECT_HEADERS_PRIVATE ${PROJECT_HEADERS_PRIVATE}
            backend/winrt/permissionwinrt.h
            backend/winrt/typeswinrt.h
        )

        set(PROJECT_SOURCES ${PROJECT_SOURCES}
            backend/winrt/permissionwinrt.cpp
            backend/winrt/typeswinrt.cpp
        )

        find_package(cppwinrt CONFIG REQUIRED)
        set(PROJECT_LINKING_LIB ${PROJECT_LINKING_LIB}
            Microsoft::CppWinRT
            windowsapp
        )
    else()
        message(STATUS "Project \"${PROJECT_NAME}\" have enabled pre-windows10 compatibility")
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(PROJECT_HEADERS_PRIVATE ${PROJECT_HEADERS_PRIVATE}
        backend/corewlan/enginecorewlan.h
        backend/corewlan/permissioncorewlan.h
    )

    set(PROJECT_SOURCES ${PROJECT_SOURCES}
        backend/corewlan/enginecorewlan.mm
        backend/corewlan/permissioncorewlan.mm
    )

    set(PROJECT_LINKING_LIB ${PROJECT_LINKING_LIB}
        "-framework CoreFoundation"
        "-framework Foundation"
        "-framework CoreWLAN"
        "-framework CoreLocation"
    )
endif()

# Manage options
## Example: if(QWLANMANAGER_USE_CUSTOM_BEHAVIOUR)

### Prepend all files to proper directory
list(TRANSFORM PROJECT_HEADERS_PUBLIC PREPEND "${PROJECT_DIR_PUBLIC_HEADERS}/")
list(TRANSFORM PROJECT_HEADERS_PRIVATE PREPEND "${PROJECT_DIR_PRIV_SOURCES}/")
list(TRANSFORM PROJECT_SOURCES PREPEND "${PROJECT_DIR_PRIV_SOURCES}/")

set(PROJECT_FILES ${PROJECT_RESSOURCES} ${PROJECT_FILE_GLOBAL} ${PROJECT_HEADERS_PUBLIC} ${PROJECT_HEADERS_PRIVATE} ${PROJECT_SOURCES})

# Add files to the library
add_library(${PROJECT_NAME} SHARED ${PROJECT_FILES})

# Set version of library
set_target_properties(${PROJECT_NAME} PROPERTIES
                        VERSION ${PROJECT_VERSION}
                        SOVERSION ${PROJECT_VERSION_MAJOR})

# Link needed libraries
target_link_directories(${PROJECT_NAME} PUBLIC ${PROJECT_LINKING_DIR})
target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${PROJECT_LINKING_LIB})

# Specify compiler options
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror=return-type -Wshadow -Wlogical-op -Wduplicated-cond -Wduplicated-branches>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror=return-type -Wshadow -Wlogical-op -Wduplicated-cond -Wduplicated-branches>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /wd4251>
)

# Compile needed definitions
## Library related
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_DEFS_PUB})
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_DEFS_PRIV})

# Specify include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_DIR_PUBLIC_HEADERS_ROOT}  # Only expose public headers
    PRIVATE
        ${PROJECT_DIR_PRIV_SOURCES}         # Internal headers, only for internal use
)

# Do we need to build tests ?
if(QWLANMANAGER_BUILD_TESTS)
    add_subdirectory(tests)
endif()
