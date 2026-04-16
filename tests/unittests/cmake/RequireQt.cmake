# SPDX-FileCopyrightText: 2025, German Aerospace Center (DLR)
# SPDX-License-Identifier: BSD-3-Clause

# ==============================================================================
# require_qt(
#   COMPONENTS <comp>...
# )
#
# Summary:
#   Locate and configure a single Qt major version (5 or 6) for the entire
#   build and ensure the requested components are available.
#
# Behavior:
#   - Uses Qt’s “dual version” pattern:
#       find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS <components>)
#       find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS <components>)
#
#   - On the first call, if QT_VERSION_MAJOR is NOT defined:
#       * If Qt has already been found by the parent project
#         (e.g. via find_package(QT ...) or find_package(Qt6/Qt5 ...)),
#         QT_VERSION_MAJOR is taken from the existing Qt configuration or
#         inferred from existing Qt6::*/Qt5::* targets.
#       * Otherwise, require_qt() calls:
#             find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS <components>)
#         which sets QT_VERSION_MAJOR to 5 or 6 according to what CMake
#         finds on the search path.
#       * QT_VERSION_MAJOR is then cached so all subsequent calls reuse
#         the same major version.
#
#   - On subsequent calls (QT_VERSION_MAJOR already defined):
#       * require_qt() simply calls:
#             find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS <components>)
#         to make sure the requested modules for the chosen major are available.
#
# Guarantees:
#   - All callers (core and modules) in a single build tree use the same Qt
#     major version, as determined by the first successful Qt detection.
#   - If the requested Qt components for that major cannot be found,
#     configuration fails with an error.
#
# User control of the chosen Qt version:
#   - Standard CMake mechanisms apply. The user can steer which Qt is found by:
#       * Adjusting CMAKE_PREFIX_PATH / Qt installation order (preferred), or
#       * Setting Qt5_DIR / Qt6_DIR to point at a specific Qt installation
#
# Notes:
#   - This function does NOT create versionless Qt:: targets. Callers are
#     expected to link against versioned targets, e.g.:
#         Qt${QT_VERSION_MAJOR}::Core
#         Qt${QT_VERSION_MAJOR}::Widgets
#         Qt${QT_VERSION_MAJOR}::<OtherComponent>
# ==============================================================================
function(require_qt)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs COMPONENTS)
    cmake_parse_arguments(RQT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT RQT_COMPONENTS)
        message(FATAL_ERROR "require_qt() called without COMPONENTS")
    endif()
    
    # --------------------------------------------------------
    # 1. Decide QT_VERSION_MAJOR once
    # --------------------------------------------------------
    if (NOT DEFINED QT_VERSION_MAJOR)

        # 1a) If user hinted a specific major via Qt6_DIR / Qt5_DIR, respect that
        if (DEFINED Qt6_DIR AND NOT DEFINED Qt5_DIR)
            set(QT_VERSION_MAJOR 6)
        elseif (DEFINED Qt5_DIR AND NOT DEFINED Qt6_DIR)
            set(QT_VERSION_MAJOR 5)

        elseif (DEFINED Qt5_DIR AND DEFINED Qt6_DIR)
            set(QT_VERSION_MAJOR 6)
        else()
            # 1b) No specific *_DIR hints: use the standard Qt dual-version pattern
            #     This will honor QT_DIR, CMAKE_PREFIX_PATH, etc.
            find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${RQT_COMPONENTS})
        endif()

        # One-time status message
        if (NOT DEFINED GTLAB_QT_VERSION_REPORTED AND DEFINED QT_VERSION_MAJOR)
            message(STATUS "GTlab: using Qt${QT_VERSION_MAJOR} for this build")
            set(GTLAB_QT_VERSION_REPORTED TRUE CACHE INTERNAL
                "Whether the selected Qt version has been reported")
        endif()
    endif()

    if (NOT DEFINED QT_VERSION_MAJOR)
        message(FATAL_ERROR
            "require_qt(): QT_VERSION_MAJOR is still undefined after detection. "
            "Check your Qt hints: QT_DIR, Qt5_DIR, Qt6_DIR, CMAKE_PREFIX_PATH.")
    endif()

    # actually find qt
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${RQT_COMPONENTS})
    set (QT_VERSION_MAJOR ${QT_VERSION_MAJOR} PARENT_SCOPE)
endfunction()