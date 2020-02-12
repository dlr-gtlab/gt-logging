cp features/gitlab_linux_local_settings.pri local_settings.pri

#C:\\Qt\\5.4\\msvc2013_64\\bin\\qmake.exe numerics.pro -r -spec win32-msvc2013 "CONFIG+=debug" "CONFIG+=qml_debug"
qmake logging.pro -spec linux-g++ CONFIG+=debug CONFIG+=qml_debug
#call "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\vcvarsall.bat" amd64
make clean
#C:\\Qt\\Tools\\QtCreator\\bin\\jom.exe
make
