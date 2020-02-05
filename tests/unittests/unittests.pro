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
LOGGING_LIB = ../../lib/logging

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

LIBS += -L$${LOGGING_LIB}

CONFIG(debug, debug|release){
LIBS += -lGTlabLogging-d
} else {
LIBS += -lGTlabLogging
}

equals(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 12) {
    message(Qt Version 5.12 or newer)
	
	CONFIG(debug, debug|release){
		LIBS += -lgtestd
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
