# *************************************************************************
# how to use
# *************************************************************************
# mkdir build
# cd build
# cmake .. -DCMAKE_BUILD_TYPE=Release  (Windows/Linux/FreeBSD)
# cmake .. -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"  (macOS)
# cmake --build . --config Release
# *************************************************************************

# *************************************************************************
# Define the versions, and options
# *************************************************************************

cmake_minimum_required(VERSION 3.24)
project(uoutil VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(default_build_type "Release")




if (WIN32)
option(WITH_ZLIB "Build internal zlib" ON)
else ()
option(WITH_ZLIB "Build internal zlib" OFF)
endif (WIN32)



if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" )
endif()


# *************************************************************************
# The basic library we are making
# *************************************************************************
add_library(uoutil STATIC
    ${PROJECT_SOURCE_DIR}/general.cpp
    ${PROJECT_SOURCE_DIR}/general.hpp
    ${PROJECT_SOURCE_DIR}/hash.cpp
    ${PROJECT_SOURCE_DIR}/hash.hpp
    ${PROJECT_SOURCE_DIR}/uop.cpp
    ${PROJECT_SOURCE_DIR}/uop.hpp
    ${PROJECT_SOURCE_DIR}/idx.cpp
    ${PROJECT_SOURCE_DIR}/idx.hpp
    ${PROJECT_SOURCE_DIR}/flag.cpp
    ${PROJECT_SOURCE_DIR}/flag.hpp
    ${PROJECT_SOURCE_DIR}/tile.cpp
    ${PROJECT_SOURCE_DIR}/tile.hpp
    ${PROJECT_SOURCE_DIR}/color.cpp
    ${PROJECT_SOURCE_DIR}/color.hpp
    ${PROJECT_SOURCE_DIR}/image.cpp
    ${PROJECT_SOURCE_DIR}/image.hpp
    ${PROJECT_SOURCE_DIR}/art.cpp
    ${PROJECT_SOURCE_DIR}/art.hpp
    ${PROJECT_SOURCE_DIR}/gump.cpp
    ${PROJECT_SOURCE_DIR}/gump.hpp
    ${PROJECT_SOURCE_DIR}/texture.cpp
    ${PROJECT_SOURCE_DIR}/texture.hpp
    ${PROJECT_SOURCE_DIR}/light.cpp
    ${PROJECT_SOURCE_DIR}/light.hpp
    ${PROJECT_SOURCE_DIR}/hue.cpp
    ${PROJECT_SOURCE_DIR}/hue.hpp
    ${PROJECT_SOURCE_DIR}/uomap.cpp
    ${PROJECT_SOURCE_DIR}/uomap.hpp
    ${PROJECT_SOURCE_DIR}/multi.cpp
    ${PROJECT_SOURCE_DIR}/multi.hpp
    ${PROJECT_SOURCE_DIR}/sound.cpp
    ${PROJECT_SOURCE_DIR}/sound.hpp
)

# *************************************************************************
# Defines and switches
# *************************************************************************

if (WIN32)

        target_compile_definitions( uoutil PRIVATE
                _LIB
                _WINDOWS
                $<$<CONFIG:Release>:NDEBUG>
                $<$<CONFIG:Debug>:_DEBUG>
        )
        
        target_compile_options( uoutil PRIVATE
                $<$<CONFIG:Release>:/O2>
        )
        
else()
        if (APPLE)
                target_compile_options( uoutil PRIVATE
                        $<$<CONFIG:Release>:-Os>
                )
        else()
                target_compile_options(uoutil PRIVATE
                        $<$<CONFIG:Release>:-O2>
                )
        endif()
endif(WIN32)


# *************************************************************************
# the directories we need on the include path
# *************************************************************************

if (WITH_ZLIB)

target_include_directories(uoutil PRIVATE
        ${PROJECT_SOURCE_DIR}/zlib
)
endif(WITH_ZLIB)
