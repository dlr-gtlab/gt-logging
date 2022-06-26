cmake_minimum_required(VERSION 3.8)
project(GTlabLogging-tests)

if (NOT TARGET GTlabLogging)
    find_package(GTlabLogging CONFIG REQUIRED)
endif()

if (NOT TARGET Qt5::Core)
    find_package(Qt5 COMPONENTS Core REQUIRED)
endif()


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (NOT TARGET gtest)
    include(AddGoogleTest)
endif()

add_executable(GTlabLoggingTests
    main.cpp
    test_basiclog.cpp
    test_disable_logger.cpp
    test_log_helper.h
    myqobject.h
    test_log_helper.cpp
    test_loglinenumbers.cpp
    test_types.cpp
    test_verbosity.cpp
)

set_property(TARGET GTlabLoggingTests PROPERTY AUTOMOC ON)

target_link_libraries(GTlabLoggingTests PRIVATE GTlabLogging gtest Qt5::Core)
