# *************************************************************************
# how to use
# *************************************************************************
# mkdir build
# cd build
# cmake .. -DCMAKE_BUILD_TYPE=Release  (Windows/Linux/FreeBSD)
# cmake .. -DCMAKE_BUILD_TYPE=Release -G"Unix Makefiles"  (macOS)
# cmake --build . --config Release
# *************************************************************************

cmake_minimum_required(VERSION 3.24)
project(uoutil VERSION 1.0 LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
option(WITH_ZLIB "Build internal zlib" OFF)


# *************************************************************************
# The basic library we are making
# *************************************************************************

if (${WITH_ZLIB} OR WIN32)
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

    ${PROJECT_SOURCE_DIR}/zlib/adler32.c
    ${PROJECT_SOURCE_DIR}/zlib/compress.c
    ${PROJECT_SOURCE_DIR}/zlib/crc32.c
    ${PROJECT_SOURCE_DIR}/zlib/deflate.c
    ${PROJECT_SOURCE_DIR}/zlib/gzclose.c
    ${PROJECT_SOURCE_DIR}/zlib/gzlib.c
    ${PROJECT_SOURCE_DIR}/zlib/gzread.c
    ${PROJECT_SOURCE_DIR}/zlib/gzwrite.c
    ${PROJECT_SOURCE_DIR}/zlib/infback.c
    ${PROJECT_SOURCE_DIR}/zlib/inffast.c
    ${PROJECT_SOURCE_DIR}/zlib/inflate.c
    ${PROJECT_SOURCE_DIR}/zlib/inftrees.c
    ${PROJECT_SOURCE_DIR}/zlib/trees.c
    ${PROJECT_SOURCE_DIR}/zlib/uncompr.c
    ${PROJECT_SOURCE_DIR}/zlib/zutil.c
    ${PROJECT_SOURCE_DIR}/zlib/crc32.h
    ${PROJECT_SOURCE_DIR}/zlib/deflate.h
    ${PROJECT_SOURCE_DIR}/zlib/gzguts.h
    ${PROJECT_SOURCE_DIR}/zlib/inffast.h
    ${PROJECT_SOURCE_DIR}/zlib/inffixed.h
    ${PROJECT_SOURCE_DIR}/zlib/inftrees.h
    ${PROJECT_SOURCE_DIR}/zlib/trees.h
    ${PROJECT_SOURCE_DIR}/zlib/zconf.h
    ${PROJECT_SOURCE_DIR}/zlib/zlib.h
    ${PROJECT_SOURCE_DIR}/zlib/zutil.h
)
else()
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
)
 
 
endif()

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
    target_compile_options( uoutil PRIVATE
        -Wno-implicit-function-declaration
    )

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


if (WITH_ZLIB OR WIN32)

target_include_directories(uoutil PUBLIC
        ${PROJECT_SOURCE_DIR}/zlib
)
endif()
