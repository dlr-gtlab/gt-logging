REM THIS IS A SCRIPT TO PREPARE AND RUN UNITTESTS

REM SET DEVTOOLS=C:\devel\GTlab-DevTools\ThirdPartyLibraries

SET PATH=%PATH%;C:\Qt\5.12.6\msvc2017_64\bin;build;lib

build\GTlabUnitTest.exe --gtest_output=xml:unittests.xml 
