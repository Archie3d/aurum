project(zlib C)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    # Disable warnings on deprecated items
    add_definitions(/wd4996)

endif()

build_static_library()

set(ZLIB_INCLUDE_DIR ${zlib_INCLUDES} CACHE PATH "ZLib library includes" FORCE)
set(ZLIB_LIBRARY_RELEASE "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/zlib.${LIB_SUFFIX}" CACHE PATH "ZLib release library" FORCE)
set(ZLIB_LIBRARY_DEBUG "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/zlib.${LIB_SUFFIX}" CACHE PATH "ZLib debug library" FORCE)
