project(kwhiteboard)

cmake_minimum_required(VERSION 2.6)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
find_package(TelepathyQt4 0.9.2 REQUIRED)

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

add_subdirectory (launcher)
add_subdirectory (kwhiteboard)
add_subdirectory (kblackboard)

macro_display_feature_log()

install(FILES approver/kblackboard.desktop approver/kwhiteboard.desktop DESTINATION ${SERVICES_INSTALL_DIR}/ktp)
