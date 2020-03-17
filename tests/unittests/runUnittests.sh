export LIBRARY_PATH=./lib/logging
export LD_LIBRARY_PATH=./lib/logging

./build/GTlabUnitTest --gtest_output=xml:unittests.xml 
