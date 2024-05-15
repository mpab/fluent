@echo off
setlocal

set BUILD_OUT=build

set PTH=%~dp0
set PTH=%PTH:~0,-1%

set PROJ=
FOR /F "delims=|" %%A IN ("%PTH%") DO (
    SET PROJ=%%~nxA
)

if "%WIN_FLEX_BISON_HOME%x"=="x" (
    echo Error - WIN_FLEX_BISON_HOME is not defined
    goto :EOF
) 

if "%BOOST_HOME%x"=="x" (
    echo Error - BOOST_HOME is not defined
    goto :EOF
) 

set INC_EXT=.\inc_win
md "%BUILD_OUT%\%PROJ%"
pushd "%BUILD_OUT%\%PROJ%"
call cmake -DCMAKE_PREFIX_PATH="%WIN_FLEX_BISON_HOME%" -DTARGET_NAME:STRING="%PROJ%" "..\..\%PROJ%"
cmake --build .
popd

:EOF
endlocal
