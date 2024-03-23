@echo off

set PRJ_SOURCE_DIR=%~dp0
set PRJ_PROJECT_DIR=%~dp0..
set PRJ_PROJECT_NAME=QTaskListExemple
set PRJ_BUILD_DIR=%PRJ_PROJECT_DIR%/build_windows
set QT_PATH=D:/Qt/6.6.2/msvc2019_64

cmake -S %PRJ_SOURCE_DIR% -B %PRJ_BUILD_DIR%