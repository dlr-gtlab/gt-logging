#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../settings.pri )

BUILD_DEST = ../build

TARGET = GTlabLogging

QT += core
TEMPLATE = lib
CONFIG += plugin
CONFIG += silent

win32 {
    DEFINES += GT_LOGGING_DLL
}

CONFIG(debug, debug|release){
    DESTDIR = $${BUILD_DEST}/debug-src/logging
    OBJECTS_DIR = $${BUILD_DEST}/debug-src/logging/obj
    MOC_DIR = $${BUILD_DEST}/debug-src/logging/moc
    RCC_DIR = $${BUILD_DEST}/debug-src/logging/rcc
    UI_DIR = $${BUILD_DEST}/debug-src/logging/ui
} else {
    DESTDIR = $${BUILD_DEST}/release-src/logging
    OBJECTS_DIR = $${BUILD_DEST}/release-src/logging/obj
    MOC_DIR = $${BUILD_DEST}/release-src/logging/moc
    RCC_DIR = $${BUILD_DEST}/release-src/logging/rcc
    UI_DIR = $${BUILD_DEST}/release-src/logging/ui
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

######################################################################
