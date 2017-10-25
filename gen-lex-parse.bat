@echo off

if "%1x" == "x" (
    set src_path=%~dp0src
) else  (
    set src_path=%1..\src
)

echo RUNNING: %0 %src_path%

start /B /D "%src_path%" /WAIT win_bison.exe --output=y.tab.cpp --defines=y.tab.h parser.y
start /B /D "%src_path%" /WAIT win_flex.exe --outfile=lex.yy.cpp --wincompat lexer.l
