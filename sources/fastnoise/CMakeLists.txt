add_library(fastnoise STATIC)

target_sources(fastnoise PUBLIC FILE_SET HEADERS BASE_DIRS ${H_SOURCES_ROOT} FILES
        generator.hpp
)

target_include_directories(fastnoise PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/fastnoise>
)

set_target_properties(fastnoise PROPERTIES LINKER_LANGUAGE CXX)