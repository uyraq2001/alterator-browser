macro(add_amb_executable name)
  # add executable
  add_executable(${name} ${ARGN})

  # add install target
  set_target_properties(${name} PROPERTIES OUTPUT_NAME ${name})
  install(TARGETS ${name} DESTINATION bin
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

  # copy binary
  add_custom_command(TARGET ${name} POST_BUILD COMMAND
      ${CMAKE_COMMAND} ARGS -E copy
        $<TARGET_FILE:${name}>
        ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:${name}>
      COMMENT "Copy ${name} to ${CMAKE_BINARY_DIR}/bin")

endmacro(add_amb_executable)

find_package(Qt5 COMPONENTS Widgets Core Gui DBus Xml LinguistTools REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    mainwindow.h
    modelbuilder.h
    categorywidget.h
    flowlayout.h
    modulepushbutton.h
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    modelbuilder.cpp
    categorywidget.cpp
    flowlayout.cpp
    modulepushbutton.cpp
)

set(UI_FORMS
    mainwindow.ui
    categorywidget.ui
)
set(RESOURCES
    resources.qrc
)

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})

qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_amb_executable(alt-diagnostic-tool ${SOURCES} ${HEADERS} ${UI_SOURCES} ${RCC_SOURCES} ${QM_FILES})

target_link_libraries(alt-diagnostic-tool Qt5::Widgets Qt5::Core Qt5::DBus Qt5::Xml Qt5::Gui ambcore)
