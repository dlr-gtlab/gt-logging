#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../../settings.pri )

TEMPLATE = app

QT += xml core sql widgets

TARGET = GTlabUnitTest

BUILD_DEST = ../../build

CONFIG(debug, debug|release){
    DESTDIR = $${BUILD_DEST}/debug-unittests
    OBJECTS_DIR = $${BUILD_DEST}/debug-unittests/obj
    MOC_DIR = $${BUILD_DEST}/debug-unittests/moc
    RCC_DIR = $${BUILD_DEST}/debug-unittests/rcc
    UI_DIR = $${BUILD_DEST}/debug-unittests/ui
} else {
    DESTDIR = $${BUILD_DEST}/release-unittests
    OBJECTS_DIR = $${BUILD_DEST}/release-unittests/obj
    MOC_DIR = $${BUILD_DEST}/release-unittests/moc
    RCC_DIR = $${BUILD_DEST}/release-unittests/rcc
    UI_DIR = $${BUILD_DEST}/release-unittests/ui
}

CONFIG += silent
CONFIG += c++11
CONFIG += console

#### INCLUDES
INCLUDEPATH += $${PWD}/../../src

#### HEADERS
HEADERS += $$files(*.h) \

#### SOURCES
SOURCES += $$files(*.cpp)

####################################################

DESTDIR = $${BUILD_DEST}

####################################################

LIBS += -L$${BUILD_DEST} -lgtest -lGTlabLogging


####################################################

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
