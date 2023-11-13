find_package(Qt5 COMPONENTS Widgets Core Gui DBus Xml LinguistTools REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(HEADERS
    object.h
    basicobject.h
    legacyobject.h
    localappobject.h
    category.h

    model.h
    categorybuilder.h
    objectitem.h
    objectsmodelbuilder.h
    desktopfileparser.h
    objectbuilderfactory.h
    objectbuilderinterface.h
    objectbuilder.h

    localapplicationbuilder.h
    localapllicationmodelbuilder.h
    localapplicationmodel.h
    localapplicationitem.h
    model.h
)

set(SOURCES
    object.cpp
    basicobject.cpp
    legacyobject.cpp
    localappobject.cpp
    category.cpp

    model.cpp
    categorybuilder.cpp
    objectitem.cpp
    objectsmodelbuilder.cpp
    desktopfileparser.cpp
    objectbuilderfactory.cpp
    objectbuilder.cpp

    localapplicationbuilder.cpp
    localapllicationmodelbuilder.cpp
    localapplicationmodel.cpp
    localapplicationitem.cpp
    model.cpp
)

add_library(ab-model ${HEADERS} ${SOURCES})

target_link_libraries(ab-model Qt5::Widgets Qt5::Core Qt5::DBus Qt5::Xml Qt5::Gui)
