
@echo off

pushd "%~dp0"

echo .
IF "%1" == "" GOTO Make
IF "%1" == "all" GOTO MakeAll

:Make
echo Compiling only changed source files (run 'build all' to compile all src files)
nmake
GOTO End

:MakeAll
echo Compiling all source files
nmake /A
GOTO End

:End
echo Deleting unnecessary files
del ..\bin\*.exp
del ..\bin\*.lib

popd