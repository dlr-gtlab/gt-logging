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

TARGET_DIR_NAME = logging
BUILD_DEST = ../lib/$${TARGET_DIR_NAME}

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
mkpath($${PWD}/include/$${TARGET_DIR_NAME})

defineTest(copyHeaders) {

    files = $$1
    dir = $${PWD}/../include/$${TARGET_DIR_NAME}
    win32:dir ~= s,/,\\,g

    win32 {

        QMAKE_POST_LINK += if not exist $$shell_quote($$dir) $$QMAKE_MKDIR $$shell_quote($$dir) $$escape_expand(\\n\\t)

        exists(*.h) {
            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote(*.h) $$shell_quote($$dir) $$escape_expand(\\n\\t)
        }

        dirNames =

        for(file, files) {

            exists($$file) {

                dirName = $$dirname(file)

                !isEmpty(dirName) {

                    !contains(dirNames, $$dirName) {

                        dirNames += $$dirName
                        sourceDir = $${PWD}/$${dirName}/*.h

                        win32:sourceDir ~= s,/,\\,g

                        exists($${sourceDir}) {

                            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($${sourceDir}) $$shell_quote($$dir) $$escape_expand(\\n\\t)
                        }
                    }
                }
            }
        }

    }
    unix {
        QMAKE_POST_LINK += find . -name $$shell_quote(*.h) -exec cp $$shell_quote({}) $$shell_quote($$dir) \; $$escape_expand(\\n\\t)
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

            win32: QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$dllPath) $$shell_quote($$environmentPath) $$escape_expand(\\n\\t)
            unix:  QMAKE_POST_LINK += find $$BUILD_DEST -name $$shell_quote(*.so*) -exec cp $$shell_quote({}) $$shell_quote($$environmentPath) \; $$escape_expand(\\n\\t)

            export(QMAKE_POST_LINK)

            return(true)
        } else {

            warning(GTLAB_ENVIRONMENT_PATH ($${environmentPath}) does not exist!)
        }
    }

    return(false)
}
######################################################################
