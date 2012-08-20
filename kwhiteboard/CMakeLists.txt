set(kwhiteboard_SRCS
    main.cpp
    kwhiteboard.cpp
    kwhiteboardwidget.cpp
    kwhiteboard-handler.cpp
    peer.cpp
)

#kde4_add_ui_files (kwhiteboard_SRCS
#)

qt4_generate_dbus_interface(org.kde.KWhiteboard.xml)
qt4_add_dbus_interface(kwhiteboard_SRCS org.kde.DBus.Peer.xml PeerInterface)
qt4_add_dbus_adaptor(kwhiteboard_SRCS org.kde.DBus.Peer.xml peer.h Peer PeerAdaptor)

kde4_add_executable (kwhiteboard
                     ${kwhiteboard_SRCS}
)

target_link_libraries(kwhiteboard
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTDBUS_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
                      ${KDE4_KDEUI_LIBS}
                      ${TELEPATHY_QT4_LIBRARIES}
                      ${QT_QTNETWORK_LIBRARY}
                      ${KTP_LIBRARIES}
)

configure_file(org.freedesktop.Telepathy.Client.KTp.KWhiteboard.service.in
               org.freedesktop.Telepathy.Client.KTp.KWhiteboard.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.freedesktop.Telepathy.Client.KTp.KWhiteboard.service
        DESTINATION ${DBUS_SERVICES_INSTALL_DIR})

install(FILES KTp.KWhiteboard.client DESTINATION ${CMAKE_INSTALL_PREFIX}/share/telepathy/clients/)

install( TARGETS kwhiteboard DESTINATION ${BIN_INSTALL_DIR})

install( FILES kwhiteboardui.rc DESTINATION  ${DATA_INSTALL_DIR}/kwhiteboard )
