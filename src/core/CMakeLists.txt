find_package(Qt5 REQUIRED COMPONENTS Widgets Core DBus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set (HEADERS
)

set (SOURCES
)

add_library(altcenter-core ${SOURCES} ${HEADERS})

add_subdirectory(logger)
target_link_libraries(altcenter-core logger)

target_link_libraries(altcenter-core Qt5::Core Qt5::DBus Qt5::Widgets)
