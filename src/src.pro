#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../settings.pri )

BUILD_DEST     = ../$${LIB_BUILD_DEST}
MOC_BUILD_DEST = ../build

CONFIG(debug, debug|release){
TARGET = GTlabLogging-d
} else {
TARGET = GTlabLogging
}

QT -= core
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent
CONFIG += c++14

win32 {
    DEFINES += GT_LOGGING_DLL
    DEFINES += GT_LOG_WIN_PRINTF_CONSOLE
}

CONFIG(debug, debug|release){
    DESTDIR = $${MOC_BUILD_DEST}/debug-src/$${TARGET_DIR_NAME}
    OBJECTS_DIR = $${MOC_BUILD_DEST}/debug-src/$${TARGET_DIR_NAME}/obj
    MOC_DIR = $${MOC_BUILD_DEST}/debug-src/$${TARGET_DIR_NAME}/moc
    RCC_DIR = $${MOC_BUILD_DEST}/debug-src/$${TARGET_DIR_NAME}/rcc
    UI_DIR = $${MOC_BUILD_DEST}/debug-src/$${TARGET_DIR_NAME}/ui
} else {
    DESTDIR = $${MOC_BUILD_DEST}/release-src/$${TARGET_DIR_NAME}
    OBJECTS_DIR = $${MOC_BUILD_DEST}/release-src/$${TARGET_DIR_NAME}/obj
    MOC_DIR = $${MOC_BUILD_DEST}/release-src/$${TARGET_DIR_NAME}/moc
    RCC_DIR = $${MOC_BUILD_DEST}/release-src/$${TARGET_DIR_NAME}/rcc
    UI_DIR = $${MOC_BUILD_DEST}/release-src/$${TARGET_DIR_NAME}/ui
}
INCLUDEPATH += .\

DESTDIR = $${BUILD_DEST}

HEADERS += \
    gt_logdest.h \
    gt_logdestconsole.h \
    gt_logdestfile.h \
    gt_logdestfunctor.h \
    gt_logdisablelogforfile.h \
    gt_logformatter.h \
    gt_logging_exports.h \
    gt_logging.h \
    gt_logging_qt_bindings.h \
    gt_loglevel.h \
    gt_logstream.h

SOURCES += \
    gt_logdestconsole.cpp \
    gt_logdestfile.cpp \
    gt_logging.cpp \
    gt_loglevel.cpp

unix {
    QMAKE_CXXFLAGS += -std=c++14
}

unix:{
    # suppress the default RPATH if you wish
    QMAKE_LFLAGS_RPATH=
    # add your own with quoting gyrations to make sure $ORIGIN gets to the command line unexpanded
    QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN\''
}

######################################################################

copyHeaders($$HEADERS)
copyToEnvironmentPath()

######################################################################
