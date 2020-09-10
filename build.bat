@echo off
set libs=/link /SUBSYSTEM:WINDOWS /LIBPATH:"C:\Program Files\boost_1_73_0\stage\lib" .\libs\BearLibTerminal.lib

@echo .
@echo Starting compile
cl /std:c++17 /MT /O2 /Fe".\bin\A-and-E-Worlds-of-Dragons" /EHsc /I "C:\Program Files\boost_1_73_0" .\src\*.cpp .\src\*.c .\src\map_generators\*.cpp .\src\game_states\*.cpp %libs%

@echo .
echo Deleting build intermediary files
del *.obj
del bin\*.exp
del bin\*.lib