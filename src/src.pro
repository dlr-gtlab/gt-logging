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

QT += core
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent

win32 {
    DEFINES += GT_LOGGING_DLL
    DEFINES += QSLOG_IS_SHARED_LIBRARY
    DEFINES += QS_LOG_WIN_PRINTF_CONSOLE
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
    gt_logging_exports.h \
    gt_logging.h \
    gt_logmodel.h \
    QsLog.h \
    QsLogDest.h \
    QsLogDestConsole.h \
    QsLogDestFile.h \
    QsLogDestFunctor.h \
    QsLogDisableForThisFile.h \
    QsLogLevel.h

SOURCES += \
    gt_logging.cpp \
    gt_logmodel.cpp \
    QsLog.cpp \
    QsLogDest.cpp \
    QsLogDestConsole.cpp \
    QsLogDestFile.cpp \
    QsLogDestFunctor.cpp

unix {
QMAKE_CXXFLAGS += -std=c++11
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
