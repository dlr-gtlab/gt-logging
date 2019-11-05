#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( local_settings.pri )

TEMPLATE = subdirs
CONFIG += ordered console
CONFIG += c++11

SUBDIRS += src

#### UNIT TESTS ####
contains(BUILD_UNITTESTS, true) {
    message("BUILD_UNITTESTS = true")
    SUBDIRS += tests/unittests
}

######################################################################
