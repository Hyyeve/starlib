add_library(nanotime STATIC)

target_sources(nanotime PUBLIC FILE_SET HEADERS BASE_DIRS ${H_SOURCES_ROOT} FILES
        nanotime.hpp
)

target_sources(nanotime PRIVATE
    nanotime.cpp
)

target_include_directories(pcg PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/nanotime>
)

set_target_properties(nanotime PROPERTIES LINKER_LANGUAGE CXX)
