SET NIGHTLYBUILDINCL=G:\AT-TW\GTlab\NightlyBuild

REM for /r .\src\ %%f in (*.h) do @copy "%%f" D:\devel\GTlab-NightlyBuild\include\logging\

for /r .\src\ %%f in (*.h) do @copy "%%f" %NIGHTLYBUILDINCL%\include\logging\
