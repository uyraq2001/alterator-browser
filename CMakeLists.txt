cmake_minimum_required(VERSION 3.0)

project("altcenter")

execute_process(
    COMMAND
    bash -c "grep 'Version:' .gear/altcenter.spec | awk -F ':' '{ print $2 }' | tr -d [:space:]"
    OUTPUT_VARIABLE ALTCENTER_GEAR_VERSION
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
message (${ALTCENTER_GEAR_VERSION})

string(REPLACE "." ";" ADT_VERSION_LIST ${ALTCENTER_GEAR_VERSION})
list(GET ALTCENTER_VERSION_LIST 0 ALTCENTER_VERSION_MAJOR)
list(GET ALTCENTER_VERSION_LIST 1 ALTCENTER_VERSION_MINOR)
list(GET ALTCENTER_VERSION_LIST 2 ALTCENTER_VERSION_PATCH)

if(UNIX)
  include(GNUInstallDirs)
else()
  if (WIN32)
    set(${CMAKE_INSTALL_LIBDIR} "lib")
    set(${CMAKE_INSTALL_DATADIR} "share")
    set(${CMAKE_INSTALL_INCLUDEDIR} "include")
    set(${CMAKE_INSTALL_BINDIR} "bin")
    message(STATUS "Setting installation destination on Windows to: ${CMAKE_INSTALL_PREFIX}")
  else()
    message(FATAL_ERROR "System not UNIX nor WIN32 - not implemented yet")
  endif()
endif()

#install(DIRECTORY /usr/share/alterator/backends
#        DESTINATION ${CMAKE_INSTALL_DATADIR}/alterator)
#install(DIRECTORY /usr/share/alterator/module-info
#        DESTINATION ${CMAKE_INSTALL_DATADIR}/alterator)
#install(DIRECTORY /usr/share/alterator/desktop-directories
#        DESTINATION ${CMAKE_INSTALL_DATADIR}/alterator)

add_subdirectory(src)

