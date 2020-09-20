Objs = draw_funcs.obj entity.obj fov.obj game_fio.obj game.obj pathfinder.obj \
       settings.obj world_gen.obj world.obj AStar.obj Grid.obj Node.obj \
	   createworld_state.obj loadworld_state.obj mainmenu_state.obj \
	   play_state.obj state_funcs.obj area.obj map.obj perlin_generator.obj \
	   map_menu.obj button.obj menu.obj panel_status.obj \
	   main.obj

SRC_DIR = ../src

TARGET  = ../bin/A-and-E-Worlds-of-Dragons.exe
 
all: $(TARGET)

$(TARGET):$(Objs)
 link /out:$@ /SUBSYSTEM:WINDOWS /LIBPATH:"C:\Program Files\boost_1_73_0\stage\lib" ..\libs\BearLibTerminal.lib $**

# src

draw_funcs.obj:$(SRC_DIR)/draw_funcs.cpp $(SRC_DIR)/draw_funcs.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/draw_funcs.cpp

entity.obj:$(SRC_DIR)/entity.cpp $(SRC_DIR)/game.h $(SRC_DIR)/entity.h $(SRC_DIR)/world.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/base.h $(SRC_DIR)/fov.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/entity.cpp

fov.obj:$(SRC_DIR)/fov.c $(SRC_DIR)/fov.h
  cl /c /Z7 /MT /O2 /EHsc $(SRC_DIR)/fov.c
  
game_fio.obj:$(SRC_DIR)/game_fio.cpp $(SRC_DIR)/game_fio.h $(SRC_DIR)/entity.h $(SRC_DIR)/game.h $(SRC_DIR)/world.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/game_fio.cpp

game.obj:$(SRC_DIR)/game.cpp $(SRC_DIR)/game.h $(SRC_DIR)/entity.h $(SRC_DIR)/world.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/settings.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/game.cpp

main.obj:$(SRC_DIR)/main.cpp $(SRC_DIR)/game_states/mainmenu_state.h $(SRC_DIR)/game.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/main.cpp

pathfinder.obj:$(SRC_DIR)/pathfinder.cpp $(SRC_DIR)/pathfinder.h $(SRC_DIR)/entity.h $(SRC_DIR)/world.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/AStar/AStar.h  $(SRC_DIR)/AStar/Grid.h  $(SRC_DIR)/AStar/Node.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/pathfinder.cpp

settings.obj:$(SRC_DIR)/settings.cpp $(SRC_DIR)/settings.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/entity.h $(SRC_DIR)/fov.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/settings.cpp

world_gen.obj:$(SRC_DIR)/world_gen.cpp $(SRC_DIR)/world_gen.h $(SRC_DIR)/map_generators/perlin_generator.h $(SRC_DIR)/game.h $(SRC_DIR)/entity.h $(SRC_DIR)/world.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/map/dungeon.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/world_gen.cpp

world.obj:$(SRC_DIR)/world.cpp $(SRC_DIR)/world.h $(SRC_DIR)/game.h $(SRC_DIR)/entity.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/world.cpp

# src/AStar

AStar.obj:$(SRC_DIR)/AStar/AStar.cpp $(SRC_DIR)/AStar/AStar.h $(SRC_DIR)/AStar/Grid.h $(SRC_DIR)/AStar/Node.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/AStar/AStar.cpp

Grid.obj:$(SRC_DIR)/AStar/Grid.cpp $(SRC_DIR)/AStar/Grid.h $(SRC_DIR)/AStar/Node.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/AStar/Grid.cpp

Node.obj:$(SRC_DIR)/AStar/Node.cpp $(SRC_DIR)/AStar/Node.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/AStar/Node.cpp

# src/game_states

createworld_state.obj:$(SRC_DIR)/game_states/createworld_state.cpp $(SRC_DIR)/game_states/createworld_state.h $(SRC_DIR)/game_states/play_state.h $(SRC_DIR)/game_states/state_funcs.h $(SRC_DIR)/draw_funcs.h $(SRC_DIR)/base.h $(SRC_DIR)/game.h $(SRC_DIR)/gamestate.h $(SRC_DIR)/world_gen.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/game_states/createworld_state.cpp

loadworld_state.obj:$(SRC_DIR)/game_states/loadworld_state.cpp $(SRC_DIR)/game_states/loadworld_state.h $(SRC_DIR)/game_states/play_state.h $(SRC_DIR)/game_states/state_funcs.h $(SRC_DIR)/draw_funcs.h $(SRC_DIR)/base.h $(SRC_DIR)/game.h $(SRC_DIR)/gamestate.h $(SRC_DIR)/game_fio.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/game_states/loadworld_state.cpp

mainmenu_state.obj:$(SRC_DIR)/game_states/mainmenu_state.cpp $(SRC_DIR)/game_states/mainmenu_state.h $(SRC_DIR)/game_states/createworld_state.h $(SRC_DIR)/game_states/loadworld_state.h $(SRC_DIR)/draw_funcs.h $(SRC_DIR)/base.h $(SRC_DIR)/game.h $(SRC_DIR)/gamestate.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/game_states/mainmenu_state.cpp

play_state.obj:$(SRC_DIR)/game_states/play_state.cpp $(SRC_DIR)/game_states/play_state.h $(SRC_DIR)/draw_funcs.h $(SRC_DIR)/base.h $(SRC_DIR)/game.h $(SRC_DIR)/gamestate.h $(SRC_DIR)/game_fio.h $(SRC_DIR)/entity.h $(SRC_DIR)/world.h $(SRC_DIR)/map/area.h $(SRC_DIR)/map/map.h $(SRC_DIR)/map/dungeon.h $(SRC_DIR)/pathfinder.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/game_states/play_state.cpp

state_funcs.obj:$(SRC_DIR)/game_states/state_funcs.cpp $(SRC_DIR)/game_states/state_funcs.h $(SRC_DIR)/game.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/game_states/state_funcs.cpp

# src/map

area.obj:$(SRC_DIR)/map/area.cpp $(SRC_DIR)/map/area.h $(SRC_DIR)/map/dungeon.h $(SRC_DIR)/map/map.h $(SRC_DIR)/entity.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/map/area.cpp

map.obj:$(SRC_DIR)/map/map.cpp $(SRC_DIR)/map/map.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/map/map.cpp

# src/map_generators

perlin_generator.obj:$(SRC_DIR)/map_generators/perlin_generator.cpp $(SRC_DIR)/map_generators/perlin_generator.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/map_generators/perlin_generator.cpp

# src/menus

map_menu.obj:$(SRC_DIR)/menus/map_menu.cpp $(SRC_DIR)/menus/map_menu.h $(SRC_DIR)/game.h $(SRC_DIR)/draw_funcs.h $(SRC_DIR)/entity.h $(SRC_DIR)/world.h $(SRC_DIR)/map/area.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/menus/map_menu.cpp

# src/ui

button.obj:$(SRC_DIR)/ui/button.cpp $(SRC_DIR)/ui/button.h $(SRC_DIR)/game.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/ui/button.cpp
  
menu.obj:$(SRC_DIR)/ui/menu.cpp $(SRC_DIR)/ui/menu.h $(SRC_DIR)/game.h $(SRC_DIR)/base.h $(SRC_DIR)/draw_funcs.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/ui/menu.cpp
  
panel_status.obj:$(SRC_DIR)/ui/panel_status.cpp $(SRC_DIR)/ui/panel_status.h $(SRC_DIR)/game.h $(SRC_DIR)/draw_funcs.h $(SRC_DIR)/entity.h $(SRC_DIR)/world.h $(SRC_DIR)/base.h
  cl /c /std:c++17 /Z7 /MT /O2 /I"C:\Program Files\boost_1_73_0" /EHsc $(SRC_DIR)/ui/panel_status.cpp