@echo off
@echo .
@echo Compiling game
nmake

@echo .
echo Deleting unnecessary files
del ..\bin\*.exp
del ..\bin\*.lib