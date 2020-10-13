

## Building with cmake

`mkdir build`
`cd build`
`cmake ..`
`cmake --build .`

The first cmake command generates the makefiles and dependencies graphs. You only need to do this step when
source files are added, or the project structure changes in some way.

The second command invokes the builder.

---

## Notes on project restructuring.

I shuffled things around a little, to bring this more in line with the way that other cross-platform projects
are structured.

bearlibterminal was moved to 3rdparty, rather than using top level include and lib directories. This will make
it cleaner if you add additional libraries later. Ideally, it would probably be better to just install that in
your system somewhere, instead of in the project itself, and use a CMake finder to hook everything up.

bin should contain scripts for manipulating the project (what you had in build). Build is for compile time
assets and shouldn't be checked into git.

The game specific code (config, saves, fonts, etc) have been moved to game. That makes it clear that these are
game assets rather than project assets. I run test builds by being in the game directory and using
`../build/A-and-E-Worlds-of-Dragons`.

## Open questions

Does the bearlibterminal library get linked on Windows? There might need to be an adjustment to the library
name on line 19 of CMakeLists.txt.

Finding the dll at runtime is still a little wonky. In the short term, it would make sense to copy it into the
game directory, but not check it into git. Or put it in a system folder somewhere on the path (I don't
remember how windows finds 3rd party dlls).

I haven't updated the scripts in bin; I wanted to make sure that the compile actually works first, before
doing those.


