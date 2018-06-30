# fennec
A C utilities library

# Building
## Mac
To build on macOS, use homebrew to install the dependencies and CMAKE. Then use the command `cmake -G "Unix Makefiles" [path]`
to actually build the framework.

## Windows
To build on windows the "simple" way is to install MSYS2. This way you can use pacman to install:

* mingw-w64-x86_64-cmake
* mingw-w64-x86_64-gcc
* mingw-w64-x86_64-make

Then you can build as if you were on UNIX with `cmake -G "MSYS Makefiles" [path]`

**Alternatively** If you really want to use Visual Studio you can make sure you have all the correct environment variables
set for the depentencies, and use `cmake -G "Visual Studio 15 2017 Win64" [path]` to get a VS2017 sln.
