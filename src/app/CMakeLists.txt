macro(add_altcenter_executable name)
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

endmacro(add_altcenter_executable)

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
    accontroller.h
    acpushbutton.h
    mainwindowsettings.h

    model/acobject.h
    model/acmodel.h
    model/acobjectcategory.h
    model/acobjectcategorybuilder.h
    model/aclocalapplication.h
    model/acobjectitem.h
    model/acobjectsmodelbuilder.h
    model/desktopfileparser.h
    model/acobjectbuilder.h

    model/aclocalapplication.h
    model/aclocalapplicationbuilder.h
    model/aclocalapllicationmodelbuilder.h
    model/aclocalapplicationmodel.h
    model/aclocalapplicationitem.h
    model/acmodel.h
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    modelbuilder.cpp
    categorywidget.cpp
    flowlayout.cpp
    accontroller.cpp
    acpushbutton.cpp
    mainwindowsettings.cpp

    model/acobject.cpp
    model/acmodel.cpp
    model/acobjectcategory.cpp
    model/acobjectcategorybuilder.cpp
    model/aclocalapplication.cpp
    model/acobjectitem.cpp
    model/acobjectsmodelbuilder.cpp
    model/desktopfileparser.cpp
    model/acobjectbuilder.cpp

    model/aclocalapplication.cpp
    model/aclocalapplicationbuilder.cpp
    model/aclocalapllicationmodelbuilder.cpp
    model/aclocalapplicationmodel.cpp
    model/aclocalapplicationitem.cpp
    model/acmodel.cpp
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

qt5_add_translation(QM_FILES ${TS_FILES})

qt5_wrap_ui(UI_SOURCES ${UI_FORMS})

qt5_add_resources(RCC_SOURCES ${RESOURCES})

add_altcenter_executable(altcenter ${SOURCES} ${HEADERS} ${UI_SOURCES} ${RCC_SOURCES} ${QM_FILES})

target_link_libraries(altcenter Qt5::Widgets Qt5::Core Qt5::DBus Qt5::Xml Qt5::Gui altcentercore)
