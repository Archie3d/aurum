project(libpng C)

set(DEPENDS zlib)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    # Disable warnings on deprecated items
    add_definitions(/wd4996)

endif()

build_static_library()

set(PNG_INCLUDE_DIR ${libpng_INCLUDES} CACHE PATH "PNG library includes" FORCE)
set(PNG_PNG_INCLUDE_DIR ${libpng_INCLUDES} CACHE PATH "PNG library includes" FORCE)
set(PNG_LIBRARY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/libpng.${LIB_SUFFIX}" CACHE PATH "PNG library" FORCE)
