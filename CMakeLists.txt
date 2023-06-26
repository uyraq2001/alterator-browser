cmake_minimum_required(VERSION 3.0)

project("Alterator Module Browser")

execute_process(
    COMMAND
    bash -c "grep 'Version:' .gear/alt-diagnostic-tool.spec | awk -F ':' '{ print $2 }' | tr -d [:space:]"
    OUTPUT_VARIABLE ADT_GEAR_VERSION
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
message (${ADT_GEAR_VERSION})

string(REPLACE "." ";" ADT_VERSION_LIST ${ADT_GEAR_VERSION})
list(GET ADT_VERSION_LIST 0 GPUI_VERSION_MAJOR)
list(GET ADT_VERSION_LIST 1 GPUI_VERSION_MINOR)
list(GET ADT_VERSION_LIST 2 GPUI_VERSION_PATCH)

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

add_subdirectory(src)

