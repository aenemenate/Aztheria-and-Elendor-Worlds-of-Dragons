
@echo off

pushd "%~dp0"

echo .

IF NOT EXIST build(
  mkdir build
)

cd build
cmake ..
cmake --build .

popd
