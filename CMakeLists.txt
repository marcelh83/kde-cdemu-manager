cmake_minimum_required(VERSION 3.16)
project(kde_cdemu)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMInstallIcons)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

kde_enable_exceptions()

find_package(KF5 REQUIRED COMPONENTS ConfigWidgets DBusAddons I18n Notifications XmlGui)
find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

set(KDE_CDEMU_VERSION "0.8.2")

add_subdirectory(src)
add_subdirectory(po)
