@echo off
set libs=/LIBPATH:"C:\Program Files\boost_1_73_0\stage\lib" .\libs\BearLibTerminal.lib
set buildpath=.\engine\*.cpp .\engine\*.c .\engine\ui\*.cpp .\engine\map_generators\*.cpp .\engine\AStar\*.cpp

@echo .
@echo Compiling engine
cl /std:c++17 /c /MT /O2 /EHsc /I "C:\Program Files\boost_1_73_0" %buildpath%
lib /out:".\libs\Engine.lib" *.obj

@echo .
echo Deleting object files
del *.obj
del bin\*.exp
del bin\*.lib