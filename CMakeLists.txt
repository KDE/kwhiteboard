project(kwhiteboard)

cmake_minimum_required(VERSION 2.6)
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(Qt4 REQUIRED)
find_package(KDE4 REQUIRED)
find_package(TelepathyQt4 REQUIRED)

include (KDE4Defaults)
include (MacroLibrary)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${CMAKE_SOURCE_DIR}
  ${TELEPATHY_QT4_INCLUDE_DIR}
  )

add_subdirectory (src)

