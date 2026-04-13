set(H_SOURCES_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(H_TARGETS
        starlib pcg nanotime stblib fastnoise
)

add_subdirectory(sources/starlib)
add_subdirectory(sources/pcg)
add_subdirectory(sources/nanotime)
add_subdirectory(sources/stblib)
add_subdirectory(sources/fastnoise)
