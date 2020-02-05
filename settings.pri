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

## FUNCTION DEFINITION FOR COPY FUNCTION
defineTest(copyHeaders) {

    files = $$1
    dir = $${PWD}/../include/$${TARGET_DIR_NAME}
    win32:dir ~= s,/,\\,g

    !exists($$dir) {

        QMAKE_POST_LINK += $$QMAKE_MKDIR $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    for(file, files) {

        exists($$file) {

            win32:file ~= s,/,\\,g
            #message(Copy: $$file)
            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
        }
    }

    export(QMAKE_POST_LINK)

    return(true)
}

defineTest(copyToEnvironmentPathModules) {
    environmentPath = $${GTLAB_ENVIRONMENT_PATH}/modules

    copyToEnvironmentPath($$1, $$environmentPath)
}

defineTest(copyToEnvironmentPath) {

    !isEmpty(GTLAB_ENVIRONMENT_PATH) {

        dllPath = $$1
        win32:dllPath ~= s,/,\\,g

        args = $$ARGS

        count(args, 2) {
            environmentPath = $$2
        } else {
            environmentPath = $${GTLAB_ENVIRONMENT_PATH}
        }

        win32:environmentPath ~= s,/,\\,g

        exists($$environmentPath) {

            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$dllPath) $$shell_quote($$environmentPath) $$escape_expand(\\n\\t)

            export(QMAKE_POST_LINK)

            return(true)
        } else {
            warning(GTLAB_ENVIRONMENT_PATH ($${environmentPath}) does not exist!)
        }
    }

    return(false)
}

######################################################################
