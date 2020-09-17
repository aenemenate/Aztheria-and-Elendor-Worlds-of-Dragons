@echo off
set libs=/link /SUBSYSTEM:WINDOWS /LIBPATH:"C:\Program Files\boost_1_73_0\stage\lib" .\libs\BearLibTerminal.lib
set buildpath=.\src\*.cpp .\src\*.c .\src\game_states\*.cpp .\src\map\*.cpp .\src\menus\*.cpp .\src\map_generators\*.cpp .\src\ui\*.cpp .\src\AStar\*.cpp

@echo .
@echo Compiling game
cl /std:c++17 /Z7 /MT /O2 /Fe".\bin\A-and-E-Worlds-of-Dragons" /EHsc /I "C:\Program Files\boost_1_73_0" %buildpath% %libs%

@echo .
echo Deleting unnecessary files
del *.obj
del bin\*.exp
del bin\*.lib