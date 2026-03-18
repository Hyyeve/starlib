set(H_SOURCES_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/sources)
set(H_TARGETS
        starlib pcg
)

add_subdirectory(sources/starlib)
add_subdirectory(sources/pcg)
