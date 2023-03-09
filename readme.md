## **UOUTIL**  
***UOUTIL*** is a c++ library to aid in developing tools for managing the assets of Ultima Online. It provides general
routines, strutures, and methods to facilating reading and writing the various binary data files (uop, mul, idx/mul).

## Obtaining the code
***UOUTIL*** includes the zlib library source in its reporistory as a submodule. Ensure one obtains the source code with:  
- `git clone --recurse-submodules https://github.com/UO-utilities/uoutil.git`  

If you have all ready cloned the repo, and did not get the zlib files, then enter:  
- `git submodule update --init --recursive` 

## Formats
***UOUTIL*** provides routines to aid in extracting and inserting data into idx/mul, mul only, and uop format.

## Assets
***UOUTIL*** supports the following:
- artwork for terrain and art tiles 
- gump artwork 
- hues 
- lights  
- multi information 
- sound 
- textures 
- tile information 

## Building
To build the library, requires a c++ compilier capable of c++17.  A CMake build file is included, and is the supported way of building the library.
The build process will generate a static library that can be linked with.  
The UOP format used by UO, can require zlib compression for accessing some of the data.  ***UOUTIL*** includes zlib with the library, and will
build that as part of the library for systems using the Windows Operating System.  By default, for Unix systems, it does not include the 
zlib library, and is expected that the system has the z library installed for system use/linking.  If that is not case, and one wants to 
include the zlib library, then build with the define WITH_ZLIB set to on (-DWITH_ZLIB=ON).  

To build (Windows users ensure one is in a command prompt with the msvc++ variables are set, a developer prompt).    
In the top level directory, where one cloned ***UOUTIL*** into:
- `mkdir build`
- `cd build`
- `cmake .. -DCMAKE_BUILD_TYPE=Release`  (Linux/FreeBSD)
- `cmake .. -DCMAKE_BUILD_TYPE=Release  -G"NMake Makefiles"` (Windows)
- `cmake .. -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"`  (macOS)
- `cmake --build . --config Release`

