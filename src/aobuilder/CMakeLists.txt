cmake_minimum_required(VERSION 3.5)

project(aobuilder VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core DBus Widgets REQUIRED)

set(HEADERS
    constants.h

    builders/objectbulderinterface.h
    builders/objectbuilderfactory.h
    builders/baseobjectbuilder.h
    builders/localapplicationobjectbuilder.h
    builders/legacyobjectbuilder.h
    builders/categoryobjectbuilder.h
    builders/objectbuilder.h
    builders/aobuilderinterface.h
    builders/aobuilderimpl.h
    builders/newobjectbuilder.h

    datasource/datasourceinterface.h
    datasource/datasourcedbusimpl.h

    objects/object.h
    objects/basicobject.h
    objects/category.h
    objects/localappobject.h
    objects/legacyobject.h

    parsers/objectparserinterface.h
    parsers/baseobjectparser.h
)

set(SOURCES
    builders/objectbuilderfactory.cpp
    builders/baseobjectbuilder.cpp
    builders/localapplicationobjectbuilder.cpp
    builders/legacyobjectbuilder.cpp
    builders/categoryobjectbuilder.cpp
    builders/aobuilderimpl.cpp
    builders/newobjectbuilder.cpp

    datasource/datasourcedbusimpl.cpp

    objects/object.cpp
    objects/category.cpp
    objects/localappobject.cpp
    objects/legacyobject.cpp

    parsers/baseobjectparser.cpp
)

add_library(aobuilder ${HEADERS}  ${SOURCES})
target_link_libraries(aobuilder Qt5::DBus Qt5::Widgets Qt5::Core)
