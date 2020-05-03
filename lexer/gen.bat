@echo off

echo.

call gen-flex.bat

call set-env.bat
echo const char build_date[] = ^"%date% %time%^"; > "%gen_path%\build_date.h"

for /f %%f in ('dir /b %gen_path%') do (
    echo -- Generated: %%f
)

echo.

call gen-build.bat

echo.

