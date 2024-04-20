cmake_minimum_required(VERSION 3.16)
project(kde_cdemu)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_WITH_QT6 ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMInstallIcons)

include(KDEInstallDirs6)
include(KDECMakeSettings)
include(KDECompilerSettings)

kde_enable_exceptions()

find_package(KF6 REQUIRED COMPONENTS ConfigWidgets CoreAddons DBusAddons I18n Notifications StatusNotifierItem XmlGui)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(KDE_CDEMU_VERSION "0.9")

add_subdirectory(src)

ki18n_install(po)
