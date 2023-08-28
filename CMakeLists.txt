cmake_minimum_required(VERSION 3.0)

project("altcenter")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
    COMMAND
    bash -c "grep 'Version:' .gear/alterator-browser.spec | awk -F ':' '{ print $2 }' | tr -d [:space:]"
    OUTPUT_VARIABLE ALTCENTER_GEAR_VERSION
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
message (${ALTCENTER_GEAR_VERSION})

string(REPLACE "." ";" ADT_VERSION_LIST ${ALTCENTER_GEAR_VERSION})
list(GET ALTCENTER_VERSION_LIST 0 ALTCENTER_VERSION_MAJOR)
list(GET ALTCENTER_VERSION_LIST 1 ALTCENTER_VERSION_MINOR)
list(GET ALTCENTER_VERSION_LIST 2 ALTCENTER_VERSION_PATCH)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(${CMAKE_MODULE_PATH}/TranslationUtils.cmake)

if(UNIX)
  include(GNUInstallDirs)
else()
    message(FATAL_ERROR "System not Linux")
endif()

install(DIRECTORY ${CMAKE_SOURCE_DIR}/backends
        DESTINATION ${CMAKE_INSTALL_DATADIR}/alterator)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/scripts
        DESTINATION ${CMAKE_INSTALL_DATADIR}/alterator)

add_subdirectory(src)

