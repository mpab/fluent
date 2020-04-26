@echo off

set proj=src

call scripts\format.bat %proj%

if not exist %proj% (
    echo ERROR: folder %proj% not found
    goto :eof
)

if not exist %proj%\CMakeLists.txt (
    echo ERROR: file %proj%\CMakeLists.txt not found
    goto :eof
)

rmdir /s /q build
mkdir build

pushd build
cmake ..\%proj%
popd