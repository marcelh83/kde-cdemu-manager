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
    KF6::ConfigWidgets
    KF6::CoreAddons
    KF6::DBusAddons
    KF6::I18n
    KF6::Notifications
    KF6::StatusNotifierItem
    KF6::XmlGui
    Qt6::Core
    Qt6::Widgets
)

install(TARGETS kde_cdemu DESTINATION ${KDE_INSTALL_BINDIR})
install(FILES kde_cdemu_mount.desktop DESTINATION ${KDE_INSTALL_DATADIR}/kio/servicemenus)
install(FILES org.kde.kde_cdemu.desktop DESTINATION ${KDE_INSTALL_APPDIR})
