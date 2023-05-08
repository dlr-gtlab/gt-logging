cmake_minimum_required(VERSION 3.12)
project(GTlabLogging VERSION 4.1.2)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR "lib")
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)


if(NOT DEFINED CMAKE_INSTALL_BINDIR)
    set(CMAKE_INSTALL_BINDIR "$<$<CONFIG:DEBUG>:binDebug>$<$<NOT:$<CONFIG:DEBUG>>:bin>")
endif(NOT DEFINED CMAKE_INSTALL_BINDIR)

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()


# GTlab uses c++11
# this needs cmake 3.1
set(CMAKE_CXX_STANDARD 14)


set(SRC
    gt_logdestconsole.cpp
    gt_logdestfile.cpp
    gt_logging.cpp
    gt_loglevel.cpp
)

SET(HDR
    gt_logdest.h
    gt_logdestconsole.h
    gt_logdestfile.h
    gt_logdestfunctor.h
    gt_logdisablelogforfile.h
    gt_logformatter.h
    gt_logging/array.h
    gt_logging/list.h
    gt_logging/map.h
    gt_logging/memory.h
    gt_logging/qt_bindings.h
    gt_logging/set.h
    gt_logging/stl_bindings.h
    gt_logging/tuple.h
    gt_logging/vector.h
    gt_logging_exports.h
    gt_logging.h
    gt_loglevel.h
    gt_logstream.h
)


add_library(GTlabLogging ${SRC} ${HDR})
add_library(GTlab::Logging ALIAS GTlabLogging)


add_compile_definitions(GTlabLogging 
   PRIVATE GT_LOGGING_DLL GTLOG_IS_SHARED_LIBRARY GT_LOG_WIN_PRINTF_CONSOLE)

target_include_directories(GTlabLogging PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include/logging>
)

# install library
install (TARGETS GTlabLogging
         EXPORT GTlabLoggingTargets
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/logging
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/logging
)

# install include files
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
        DESTINATION include/logging
        FILES_MATCHING
        PATTERN "*.h*"
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/GTlabLoggingConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)

file(GENERATE
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/GTlabLoggingConfig.cmake"
  INPUT  "${PROJECT_SOURCE_DIR}/cmake/GTlabLoggingConfig.cmake.in"
)


install (EXPORT GTlabLoggingTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GTlabLogging)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/GTlabLoggingConfigVersion.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/GTlabLoggingConfig.cmake
         DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GTlabLogging)
