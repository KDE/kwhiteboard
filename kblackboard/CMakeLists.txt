
# Insert your sources here
set(TELEPATHY-WHITEBOARD_SRCS
    telepathywhiteboard.cpp
    whiteboardwidget.cpp
    main.cpp
)


# Create the plasmoid
kde4_add_executable(telepathy-whiteboard ${TELEPATHY-WHITEBOARD_SRCS})

# Link against the needed libraries
target_link_libraries(telepathy-whiteboard
  ${TELEPATHY_QT4_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  ${KDE4_KDEUI_LIBS}
)

install(TARGETS telepathy-whiteboard ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES telepathy-whiteboard.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})

macro_display_feature_log()


