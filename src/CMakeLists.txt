cmake_minimum_required(VERSION 3.12)
project(GTlabLogging VERSION 3.0.0)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)


if(NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "$<$<CONFIG:DEBUG>:binDebug>$<$<NOT:$<CONFIG:DEBUG>>:bin>")
endif(NOT DEFINED CMAKE_INSTALL_BINDIR)

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Get Qt5
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# GTlab uses c++11
# this needs cmake 3.1
set(CMAKE_CXX_STANDARD 11)


set(SRC
    gt_logmodel.cpp
    gt_log.cpp
    gt_logdest.cpp
    gt_logdestconsole.cpp
    gt_logdestfile.cpp
    gt_logdestfunctor.cpp
)

SET(HDR
    gt_logging_exports.h
    gt_logging.h
    gt_logmodel.h
    gt_logdest.h
    gt_logdestconsole.h
    gt_logdestfile.h
    gt_logdestfunctor.h
    gt_logdisablelogforfile.h
    gt_loglevel.h
    gt_logstream.h
)


add_library(GTlabLogging ${SRC} ${HDR})
set_property(TARGET GTlabLogging PROPERTY AUTOMOC ON)

add_compile_definitions(GTlabLogging 
   PRIVATE GT_LOGGING_DLL GTLOG_IS_SHARED_LIBRARY GT_LOG_WIN_PRINTF_CONSOLE)

target_link_libraries(GTlabLogging  PUBLIC Qt5::Core Qt5::Gui Qt5::Widgets)

target_include_directories(GTlabLogging PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/GTlabLogging>
)

# install library
install (TARGETS GTlabLogging
         EXPORT GTlabLogging-targets
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install include files
install(FILES ${HDR} DESTINATION include/GTlabLogging)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/GTlabLogging-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

file(GENERATE
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/GTlabLogging-config.cmake"
  INPUT  "${PROJECT_SOURCE_DIR}/cmake/GTlabLogging-config.cmake.in"
)


install (EXPORT GTlabLogging-targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GTlabLogging)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/GTlabLogging-config-version.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/GTlabLogging-config.cmake
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GTlabLogging)
