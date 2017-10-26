@echo off

SETLOCAL EnableDelayedExpansion
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "DEL=%%a"
)

if "%DevEnvDir%x" == "x" (
    call vsvars32
)

pushd vs2015

if exist x64\Debug\ del /q x64\Debug\*
if exist x64\Release\ del /q x64\Release\*

msbuild /p:Configuration="Release" /p:Platform="x64"
msbuild /p:Configuration="Debug" /p:Platform="x64"

if exist x64\Debug\fluent.exe (
    copy x64\Debug\fluent.exe C:\Apps\bin\dfluent.exe > NUL
    call :ColorText 2E "debug build OK"
) else (
    call :ColorText 4E "debug build FAIL"
)

echo.

if exist x64\Release\fluent.exe (
    copy x64\Release\fluent.exe C:\Apps\bin\fluent.exe > NUL
    call :ColorText 2E "release build OK"
) else (
    call :ColorText 4E "release build FAIL"
)

echo .

popd
exit /B

:ColorText
echo off
<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1
goto :eof