@echo off

if "%1x" == "x" (
    set src_path=%~dp0src
) else  (
    set src_path=%1..\src
)

echo const char build_date[] = ^"%date% %time%^"; > "%src_path%\build_date.h"