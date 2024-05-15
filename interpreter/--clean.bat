@echo off
setlocal

set BUILD_OUT=build

set PTH=%~dp0
set PTH=%PTH:~0,-1%

set PROJ=
FOR /F "delims=|" %%A IN ("%PTH%") DO (
    SET PROJ=%%~nxA
)

rd /s /q "%BUILD_OUT%\%PROJ%"
