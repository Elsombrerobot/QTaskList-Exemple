@echo off

set PRJ_PROJECT_DIR=%~dp0../..
set PRJ_BUILD_DIR=%PRJ_PROJECT_DIR%/build_windows

cmake --build %PRJ_BUILD_DIR% --target update_translations
