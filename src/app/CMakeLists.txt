macro(add_alterator_browser_executable name)
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

endmacro(add_alterator_browser_executable)

find_package(Qt5 COMPONENTS Widgets Core Gui DBus Xml LinguistTools REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    mainwindow.h
    categorywidget.h
    flowlayout.h
    controller.h
    pushbutton.h
    mainwindowsettings.h

    model/object.h
    model/model.h
    model/objectcategory.h
    model/objectcategorybuilder.h
    model/localapplication.h
    model/objectitem.h
    model/objectsmodelbuilder.h
    model/desktopfileparser.h
    model/objectbuilder.h

    model/localapplication.h
    model/localapplicationbuilder.h
    model/localapllicationmodelbuilder.h
    model/localapplicationmodel.h
    model/localapplicationitem.h
    model/model.h
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    categorywidget.cpp
    flowlayout.cpp
    controller.cpp
    pushbutton.cpp
    mainwindowsettings.cpp

    model/object.cpp
    model/model.cpp
    model/objectcategory.cpp
    model/objectcategorybuilder.cpp
    model/localapplication.cpp
    model/objectitem.cpp
    model/objectsmodelbuilder.cpp
    model/desktopfileparser.cpp
    model/objectbuilder.cpp

    model/localapplication.cpp
    model/localapplicationbuilder.cpp
    model/localapllicationmodelbuilder.cpp
    model/localapplicationmodel.cpp
    model/localapplicationitem.cpp
    model/model.cpp
)

set(UI_FORMS
    mainwindow.ui
    categorywidget.ui
)
set(RESOURCES
    resources.qrc
)
set(TS_FILES
${CMAKE_CURRENT_SOURCE_DIR}/ru.ts
${CMAKE_CURRENT_SOURCE_DIR}/en.ts
)

add_custom_target(app_ts
DEPENDS ${TS_FILES}
COMMAND ${Qt5_LUPDATE_EXECUTABLE} -recursive ${CMAKE_CURRENT_SOURCE_DIR} -ts ${TS_FILES} -noobsolete
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR})

add_translation(QM_FILES ${TS_FILES})

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})

add_translation_resource(RESOURCES_SRC "alterator-browser" ${QM_FILES})

qt5_add_resources(RCC_SOURCES ${RESOURCES} ${RESOURCES_SRC})

add_alterator_browser_executable(alterator-browser ${SOURCES} ${HEADERS} ${UI_SOURCES} ${RCC_SOURCES})

target_link_libraries(alterator-browser alterator-browser-core Qt5::Widgets Qt5::Core Qt5::DBus Qt5::Xml Qt5::Gui)
