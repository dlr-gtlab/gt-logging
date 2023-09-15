cmake_minimum_required(VERSION 3.8)
project(GTlabLogging-tests)

if (NOT TARGET GTlab::Logging)
    find_package(GTlabLogging CONFIG REQUIRED)
endif()

if (NOT TARGET Qt5::Core)
    find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)
endif()


list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (NOT TARGET gtest)
    include(AddGoogleTest)
endif()

add_executable(GTlabLoggingTests
    main.cpp
    test_helper.h
    test_log_helper.h
    test_logdest.cpp
    test_logdestfile.cpp
    test_logdisableforfile.cpp
    test_logformatter.cpp 
    test_logid.cpp  
    test_loglevel.cpp  
    test_loglinenumbers.cpp
    test_logonce.cpp
    test_logquote.cpp
    test_logstatesaver.cpp
    test_types.cpp
    test_types_qt.cpp
    test_verbosity.cpp
)

set_property(TARGET GTlabLoggingTests PROPERTY AUTOMOC ON)

target_link_libraries(GTlabLoggingTests PRIVATE GTlab::Logging gtest Qt5::Core Qt5::Gui Qt5::Widgets)
