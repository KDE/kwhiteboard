project(kwhiteboard)

cmake_minimum_required(VERSION 2.6)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
find_package(TelepathyQt4 0.9.1 REQUIRED)

set(IS_KTP_INTERNAL_MODULE TRUE)
find_package(KTp REQUIRED)

include (KDE4Defaults)
include (MacroLibrary)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${CMAKE_SOURCE_DIR}
  ${TELEPATHY_QT4_INCLUDE_DIR}
  ${KTP_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )


set(CMAKE_REQUIRED_INCLUDES "${QT_INCLUDES};${TELEPATHY_QT4_INCLUDE_DIR}")
include (CheckIncludeFileCXX)
check_include_file_cxx("TelepathyQt/DBusTubeChannel" HAS_TP_QT_DBUS)
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_subdirectory (launcher)

if (HAS_TP_QT_DBUS)
add_subdirectory (kwhiteboard)
endif (HAS_TP_QT_DBUS)

add_subdirectory (kblackboard)
