set(KBOARD_LAUNCHER_SRCS
    main.cpp
    mainwindow.cpp
)

set(KBOARD_LAUNCHER_UI
    mainwindow.ui)

kde4_add_ui_files(KBOARD_LAUNCHER_SRCS ${KBOARD_LAUNCHER_UI})

kde4_add_executable(kboard-launcher ${KBOARD_LAUNCHER_SRCS})

target_link_libraries(kboard-launcher
  ${TELEPATHY_QT4_LIBRARIES}
  ${KDE4_KDEUI_LIBS}
  ${KTP_LIBRARIES}
  ${KTP_MODELS_LIBRARIES}
  ${KTP_WIDGETS_LIBRARIES}
)

# install(TARGETS kboard-launcher ${INSTALL_TARGETS_DEFAULT_ARGS})
# install(FILES kboard-launcher.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
