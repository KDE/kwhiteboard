
# Insert your sources here
set(TELEPATHY-BLACKBOARD_SRCS
    telepathyblackboard.cpp
    blackboardwidget.cpp
    main.cpp
)


# Create the plasmoid
kde4_add_executable(telepathy-blackboard ${TELEPATHY-BLACKBOARD_SRCS})

# Link against the needed libraries
target_link_libraries(telepathy-blackboard
  ${TELEPATHY_QT4_LIBRARIES}
  ${QT_QTNETWORK_LIBRARY}
  ${KDE4_KDEUI_LIBS}
)

install(TARGETS telepathy-blackboard ${INSTALL_TARGETS_DEFAULT_ARGS})
#install(FILES telepathy-blackboard.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
