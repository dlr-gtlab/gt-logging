# SPDX-FileCopyrightText: 2023, German Aerospace Center (DLR)
# SPDX-License-Identifier: BSD-3-Clause

LIB_DIR=./lib/$TARGETDIRNAME
QT_DIR=/opt/Qt/5.12.5/gcc_64

export LIBRARY_PATH=$LIB_DIR:$LIBRARY_PATH
export LIBRARY_PATH=$LIB_DIR/modules:$LIBRARY_PATH
export LIBRARY_PATH=$QT_DIR:$LIBRARY_PATH
export LIBRARY_PATH=$QT_DIR/lib:$LIBRARY_PATH
export LIBRARY_PATH=$DEVTOOLS:$LIBRARY_PATH
export LIBRARY_PATH=$DEVTOOLS/lib:$LIBRARY_PATH
export LD_LIBRARY_PATH=$LIBRARY_PATH
./build/$UNITTESTSNAME --gtest_output=xml:unittests.xml