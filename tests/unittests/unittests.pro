#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( $${PWD}/../../settings.pri )

BUILD_DEST     = ../../build
MOC_BUILD_DEST = $${BUILD_DEST}

TEMPLATE = app

QT += xml core sql widgets

TARGET = GTlabUnitTest

CONFIG(debug, debug|release){
    DESTDIR = $${MOC_BUILD_DEST}/debug-unittests
    OBJECTS_DIR = $${MOC_BUILD_DEST}/debug-unittests/obj
    MOC_DIR = $${MOC_BUILD_DEST}/debug-unittests/moc
    RCC_DIR = $${MOC_BUILD_DEST}/debug-unittests/rcc
    UI_DIR = $${MOC_BUILD_DEST}/debug-unittests/ui
} else {
    DESTDIR = $${MOC_BUILD_DEST}/release-unittests
    OBJECTS_DIR = $${MOC_BUILD_DEST}/release-unittests/obj
    MOC_DIR = $${MOC_BUILD_DEST}/release-unittests/moc
    RCC_DIR = $${MOC_BUILD_DEST}/release-unittests/rcc
    UI_DIR = $${MOC_BUILD_DEST}/release-unittests/ui
}

CONFIG += silent
CONFIG += console
CONFIG += c++14

#### INCLUDES
INCLUDEPATH += $${PWD}/../../src

#### HEADERS
HEADERS += $$files(*.h) \

#### SOURCES
SOURCES += $$files(*.cpp) \

####################################################

DESTDIR = $${BUILD_DEST}

####################################################

LIBS += -L../../$${LIB_BUILD_DEST}

CONFIG(debug, debug|release){
LIBS += -lGTlabLogging-d
} else {
LIBS += -lGTlabLogging
}

equals(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 12) {
    message(Qt Version 5.12 or newer)

        CONFIG(debug, debug|release){
        win32 {
                LIBS += -lgtestd
        }
        unix {
                LIBS += -lgtest
        }
        } else {
                LIBS += -lgtest
        }

} else {
    message(Qt Version older than 5.12)

        LIBS += -lgtest
}

####################################################

# add search paths to shared libraries
unix: QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN:\$$ORIGIN/modules\''

######################################################################
