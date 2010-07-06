cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories(${QT_INCLUDES} ${KDE4_INCLUDES})

kde4_add_executable(kwhiteboard main.cpp kwhiteboard.cpp kwhiteboardwidget.cpp)

qt4_generate_dbus_interface(org.kde.KWhiteBoard.xml)

target_link_libraries(kwhiteboard ${KDE4_KDEUI_LIBS} ${QT_QTNETWORK_LIBRARY})

install( FILES kwhiteboardui.rc DESTINATION  ${DATA_INSTALL_DIR}/kwhiteboard )

