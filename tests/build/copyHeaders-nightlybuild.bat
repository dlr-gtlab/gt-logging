if exist "%NIGHTLYBUILD%\include\%TARGETDIRNAME%\" rmdir /s /q %NIGHTLYBUILD%\include\%TARGETDIRNAME%\
if not exist "%NIGHTLYBUILD%\include\%TARGETDIRNAME%\" mkdir %NIGHTLYBUILD%\include\%TARGETDIRNAME%\
if exist "%NIGHTLYBUILD%\lib\%TARGETDIRNAME%\" rmdir /s /q %NIGHTLYBUILD%\lib\%TARGETDIRNAME%\
if not exist "%NIGHTLYBUILD%\lib\%TARGETDIRNAME%\" mkdir %NIGHTLYBUILD%\lib\%TARGETDIRNAME%\

for /r .\include %%f in (*.h) do @copy "%%f" %NIGHTLYBUILD%\include\%TARGETDIRNAME%\
for /r .\lib %%f in (*.dll) do @copy "%%f" %NIGHTLYBUILD%\lib\%TARGETDIRNAME%\
for /r .\lib %%f in (*.lib) do @copy "%%f" %NIGHTLYBUILD%\lib\%TARGETDIRNAME%\
