@echo off

set PRJ_SOURCE_DIR=%~dp0
set PRJ_PROJECT_DIR=%~dp0..
set PRJ_PROJECT_NAME=QTaskList-Exemple
set PRJ_BUILD_DIR=%PRJ_PROJECT_DIR%/build_windows

cmake -S %PRJ_SOURCE_DIR% -B %PRJ_BUILD_DIR%