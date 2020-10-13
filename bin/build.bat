
@echo off

pushd "%~dp0"
cd "..\"

echo .

IF NOT EXIST build mkdir build

cd build
cmake ..
IF "%1" == "" msbuild aztheria_and_elendor.sln /p:Configuration=Release
IF "%1" == "/release" msbuild aztheria_and_elendor.sln /p:Configuration=Release
IF "%1" == "/debug" msbuild aztheria_and_elendor.sln /p:Configuration=Debug

popd