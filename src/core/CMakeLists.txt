find_package(Qt5 REQUIRED COMPONENTS Widgets Core DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
    adtdesktopfileparser.h
)

set (SOURCES
    adtdesktopfileparser.cpp
)

ADD_LIBRARY(ambcore STATIC ${SOURCES} ${HEADERS})
target_link_libraries(ambcore PRIVATE Qt5::Core Qt5::DBus Qt5::Widgets)
