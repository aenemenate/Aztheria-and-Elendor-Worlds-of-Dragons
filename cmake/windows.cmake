

# Compiler setup
# SET(CMAKE_CXX_FLAGS "/c /std:c++17 /Z7 /MT /O2")

# Linker setup
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
link_directories("3rdparty/bearlibterminal/lib/windows")

set(BOOST_ROOT "C:/Program Files/boost/boost_1_76_0")

