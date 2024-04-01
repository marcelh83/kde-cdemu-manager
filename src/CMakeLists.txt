include_directories(${PROJECT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})

configure_file(kdecdemuversion.h.in ${PROJECT_BINARY_DIR}/kdecdemuversion.h)

set(kde_cdemu_SRCS
    cdemu.cpp
    devicelistitem.cpp
    exception.cpp
    main.cpp
    mainwindow.cpp
    messagebox.cpp
)

set(kde_cdemu_HDRS
    cdemu.h
    devicelistitem.h
    exception.h
    mainwindow.h
    messagebox.h
)

ki18n_wrap_ui(kde_cdemu_SRCS mainwindow.ui)
add_executable(kde_cdemu ${kde_cdemu_SRCS} ${kde_cdemu_HDRS})

target_link_libraries(kde_cdemu
    KF5::ConfigWidgets
    KF5::DBusAddons
    KF5::I18n
    KF5::Notifications
    KF5::XmlGui
    Qt5::Core
    Qt5::Widgets
)

install(TARGETS kde_cdemu DESTINATION ${BIN_INSTALL_DIR})
install(FILES kde_cdemu_mount.desktop DESTINATION ${SERVICES_INSTALL_DIR}/ServiceMenus)
install(FILES org.kde.kde_cdemu.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
