#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

#### LOCAL SETTINGS
include( local_settings.pri )
include( deployment.pri )

TARGET_DIR_NAME = logging
LIB_BUILD_DEST  = lib/$${TARGET_DIR_NAME}

equals(QT_MAJOR_VERSION, 5):!lessThan(QT_MINOR_VERSION, 12) {
    message(Qt Version 5.12 or newer)
	
	# Google Test
	INCLUDEPATH += $${GOOGLE_TEST_PATH}/include
	LIBS        += -L$${GOOGLE_TEST_PATH}/lib
	DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib

} else {
    message(Qt Version older than 5.12)
	
	# Google Test
	INCLUDEPATH += $${GOOGLE_TEST_PATH}/include
	win32 {
		CONFIG(debug, debug|release){
			LIBS        += -L$${GOOGLE_TEST_PATH}/libDebug
			DEPENDPATH  += $${GOOGLE_TEST_PATH}/libDebug
		} else {
			LIBS        += -L$${GOOGLE_TEST_PATH}/lib
			DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib
		}
	}
	unix {
		LIBS        += -L$${GOOGLE_TEST_PATH}/lib
		DEPENDPATH  += $${GOOGLE_TEST_PATH}/lib
	}

}
######################################################################
