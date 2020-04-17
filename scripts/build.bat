@echo off

if "%APPS%x" == "x" (
    echo ERROR: the APPS environment variable is not set
    got :eof
)

if not exist "%APPS%" (
    echo ERROR: the "%APPS%" folder does not exist
    goto :eof
)

set install_dir="%APPS%\scripts"

if not exist "%install_dir%" (
    echo ERROR: the "%install_dir%" folder does not exist
    goto :eof
)

set WIN_FLEX_BISON_HOME=%APPS%\win_flex_bison-2.5.18

set BOOST_HOME=%APPS%\boost_1_72_0

if "%WIN_FLEX_BISON_HOME%x" == "x" (
    echo ERROR: WIN_FLEX_BISON_HOME is not set
    goto :eof
)

if not exist "%WIN_FLEX_BISON_HOME%" (
    echo ERROR: the "%WIN_FLEX_BISON_HOME%" folder does not exist
    goto :eof
)

SETLOCAL EnableDelayedExpansion
call vsenv.bat 64

pushd vs2019

if exist x64\Debug\ del /q x64\Debug\*
if exist x64\Release\ del /q x64\Release\*

msbuild /p:Configuration="Release" /p:Platform="x64"
msbuild /p:Configuration="Debug" /p:Platform="x64"

if exist x64\Debug\fluent.exe (
    copy x64\Debug\fluent.exe %install_dir%\dfluent.exe > NUL
    call :ColorText 2E "debug build OK"
) else (
    call :ColorText 4E "debug build FAIL"
)

echo.

if exist x64\Release\fluent.exe (
    copy x64\Release\fluent.exe %install_dir%\fluent.exe > NUL
    call :ColorText 2E "release build OK"
) else (
    call :ColorText 4E "release build FAIL"
)

echo.

popd
exit /B

:ColorText
echo off
<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1
goto :eof