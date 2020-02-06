#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### Version 1.0
#### DO NOT CHANGE!
######################################################################


## CREATE INCLUDE DIRECTORY
HEADERS_DIR_PATH = $${PWD}/include/
mkpath($${HEADERS_DIR_PATH})

## FUNCTION DEFINITION FOR COPY FUNCTION
defineTest(copyHeaders) {

    files = $$1
    dir = $${HEADERS_DIR_PATH}/$${TARGET_DIR_NAME}

    win32 {

        dir ~= s,/,\\,g

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

                        sourceDir ~= s,/,\\,g

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

    isEmpty(GTLAB_ENVIRONMENT_PATH) {

        return(false)
    }

    environmentPath = $${GTLAB_ENVIRONMENT_PATH}/modules

    copyToEnvironmentPath($$environmentPath)
}

defineTest(copyToEnvironmentPath) {

    isEmpty(GTLAB_ENVIRONMENT_PATH) {

        return(false)
    }

    isEmpty(BUILD_DEST) {

        return(false)
    }

    args = $$ARGS

    count(args, 1) {

        environmentPath = $$1
    } else {

        environmentPath = $${GTLAB_ENVIRONMENT_PATH}
    }

    win32:environmentPath ~= s,/,\\,g

    exists($$environmentPath) {

        win32 {
            dllPath = $${BUILD_DEST}/$${TARGET}.dll
            dllPath ~= s,/,\\,g

            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$dllPath) $$shell_quote($$environmentPath) $$escape_expand(\\n\\t)
        }

        unix:  QMAKE_POST_LINK += find $${BUILD_DEST} -name $$shell_quote(*$${TARGET}.so*) -exec cp $$shell_quote({}) $$shell_quote($$environmentPath) \; $$escape_expand(\\n\\t)

        export(QMAKE_POST_LINK)

        return(true)
    } else {

        warning(GTLAB_ENVIRONMENT_PATH ($${environmentPath}) does not exist!)
    }

    return(true)
}
######################################################################
