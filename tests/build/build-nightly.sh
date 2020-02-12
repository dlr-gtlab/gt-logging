cp features/gitlab_linux_local_settings.pri local_settings.pri

qmake logging.pro -spec linux-g++ CONFIG+=release 
make clean
make
