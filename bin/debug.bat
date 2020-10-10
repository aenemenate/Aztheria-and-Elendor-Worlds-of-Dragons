@echo off

pushd "%~dp0"

echo .
echo Opening debug environment for Aztheria and Elendor: Worlds of Dragons
cd ../bin
devenv A-and-E-Worlds-of-Dragons.exe
cd ../build

popd