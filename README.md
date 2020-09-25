# Aztheria & Elendor: Worlds of Dragons

This game is being developed for my little sister, who I hope will actually play it. :-P
Included in the repository is both the source code and the bin, as no actual release is ready to take place.

## Building the project

This repository is designed to be built under Windows with the x64 cl.exe compiler. Note that to build you must have installed boost 1.73.0 in the default location and compiled it with the x64 compiler.

Open your x64 Developer Command Prompt and navigate to the project tree.

Navigate to build, then type 'build' to build the game, and 'run' or 'debug', respectively.

Note: To build on linux you will have to change the main func from WinMain to main (or create a new main called linux_main.cpp), write a linux makefile, as well as download the linux version of BearLibTerminal. You should also have boost installed on your system. I don't currently have access to linux or I would have done this all myself already (My pc use Intel RST, which I can't easily change since my Windows install relies on it).

# Added features

* world map
* extended block functionality (plants, stairs)
* fov
* pathfinding
* save/load
