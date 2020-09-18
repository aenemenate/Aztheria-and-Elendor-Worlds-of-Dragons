Objs = draw_funcs.obj entity.obj fov.obj game_fio.obj game.obj pathfinder.obj \
       settings.obj world_gen.obj world.obj AStar.obj Grid.obj Node.obj \
	   createworld_state.obj loadworld_state.obj mainmenu_state.obj \
	   play_state.obj state_funcs.obj area.obj map.obj perlin_generator.obj \
	   map_menu.obj button.obj menu.obj panel_status.obj main.obj

TARGET  = ../bin/A-and-E-Worlds-of-Dragons.exe
 
all: $(TARGET)

$(TARGET):$(Objs)
 link /out:$@ /SUBSYSTEM:WINDOWS /LIBPATH:"C:\Program Files\boost_1_73_0\stage\lib" ..\libs\BearLibTerminal.lib $**

{../src}.cpp.obj::
 cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $<
{../src/AStar}.cpp.obj::
 cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $<
{../src/game_states}.cpp.obj::
 cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $<
{../src/map}.cpp.obj::
 cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $<
{../src/map_generators}.cpp.obj::
 cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $<
{../src/menus}.cpp.obj::
 cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $<
{../src/ui}.cpp.obj::
 cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $<
{../src}.c.obj::
 cl /c /Z7 /MT /O2 /EHsc $<
