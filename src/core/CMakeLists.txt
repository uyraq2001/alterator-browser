find_package(Qt5 REQUIRED COMPONENTS Widgets Core DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
    adtdesktopfileparser.h
    enums.h
)

set (SOURCES
    adtdesktopfileparser.cpp
)

ADD_LIBRARY(altcentercore STATIC ${SOURCES} ${HEADERS})
target_link_libraries(altcentercore PRIVATE Qt5::Core Qt5::DBus Qt5::Widgets)
