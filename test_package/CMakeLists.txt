# SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.12)
project(PackageTest CXX)

find_package(GTlabLogging REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example GTlab::Logging)
