# SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.8)
project(GTlabLogging-tests)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if (NOT TARGET GTlab::Logging)
    find_package(GTlabLogging CONFIG REQUIRED)
endif()

if (NOT TARGET Qt5::Core AND NOT TARGET Qt6::Core)
    include(RequireQt)
    require_qt(COMPONENTS Core Gui Widgets)
endif()

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

target_link_libraries(GTlabLoggingTests PRIVATE GTlab::Logging gtest Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets)

include(GoogleTest)
gtest_discover_tests(GTlabLoggingTests TEST_PREFIX "Logging." DISCOVERY_MODE PRE_TEST)
