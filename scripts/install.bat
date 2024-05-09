set MSBUILD_EXEC="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
set FILE=%~dp0../..\build_windows\INSTALL.vcxproj

set CONFIG=%1

set "CONFIG=%~1"
if "%CONFIG%"=="" set "CONFIG=release"

%MSBUILD_EXEC% %FILE% /p:Configuration=%CONFIG%