@echo off
call set-env.bat
rmdir /s /q %build_path%
mkdir %build_path%
pushd %build_path%
cmake ..
popd