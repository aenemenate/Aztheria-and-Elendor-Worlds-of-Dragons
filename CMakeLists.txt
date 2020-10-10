cmake_minimum_required(VERSION 3.10.0)
project("aztheria_and_elendor")

# Compiler settings. These might need to be platform specific?
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -stdlib=libc++")

# Setup boost
find_package(Boost REQUIRED COMPONENTS serialization)
include_directories(${Boost_INCLUDE_DIRS})

# Setup bearlibterminal
include_directories("3rdparty/bearlibterminal/include")
if(WIN32)
  link_directories("3rdparty/bearlibterminal/lib/windows")
elseif(APPLE)
  link_directories("3rdparty/bearlibterminal/lib/macos")
endif()
set(BearLibTerminal_LIBRARIES "BearLibTerminal")

# Specify all the code to compile
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Build it all
add_executable(aztheria ${SOURCES})
target_link_libraries(aztheria ${Boost_LIBRARIES} ${BearLibTerminal_LIBRARIES})


