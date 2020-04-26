@echo off 

pushd %1
call :formatcpp
popd

goto :eof

:formatcpp
for /r %%i in (*.cpp) do call clang-format -i -style=WebKit %%i
for /r %%i in (*.h) do call clang-format -i -style=WebKit %%i
goto :eof