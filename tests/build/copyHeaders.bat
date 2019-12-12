SET NIGHTLYBUILD=G:\AT-TW\GTlab\Nightly_Builds

if not exist "%NIGHTLYBUILD%\include\logging\" mkdir %NIGHTLYBUILD%\include\logging\
if not exist "%NIGHTLYBUILD%\lib\logging\" mkdir %NIGHTLYBUILD%\lib\logging\

for /r .\include %%f in (*.h) do @copy "%%f" %NIGHTLYBUILD%\include\logging\
for /r .\lib %%f in (*.dll) do @copy "%%f" %NIGHTLYBUILD%\lib\logging\
for /r .\lib %%f in (*.lib) do @copy "%%f" %NIGHTLYBUILD%\lib\logging\
