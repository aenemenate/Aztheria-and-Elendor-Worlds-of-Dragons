
TARGET  = ../bin/A-and-E-Worlds-of-Dragons.exe
LD_FLAGS = /DEBUG:FULL /SUBSYSTEM:WINDOWS /LIBPATH:"C:\Program Files\boost_1_73_0\stage\lib" ..\libs\BearLibTerminal.lib
CPP_FLAGS = /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0"

$(TARGET):*.obj
 link /out:$@ $(LD_FLAGS) *.obj

{../src}.cpp.obj::
 cl /c $(CPP_FLAGS) /EHsc $<
{../src}.c.obj::
 cl /c /Z7 /MT /O2 /EHsc $<
{../src/AStar}.cpp.obj::
 cl /c $(CPP_FLAGS) /EHsc $<
{../src/game_states}.cpp.obj::
 cl /c $(CPP_FLAGS) /EHsc $<
{../src/map}.cpp.obj::
 cl /c $(CPP_FLAGS) /EHsc $<
{../src/map_generators}.cpp.obj::
 cl /c $(CPP_FLAGS) /EHsc $<
{../src/menus}.cpp.obj::
 cl /c $(CPP_FLAGS) /EHsc $<
{../src/ui}.cpp.obj::
 cl /c $(CPP_FLAGS) /EHsc $<
