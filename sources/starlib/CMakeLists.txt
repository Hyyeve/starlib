add_library(starlib STATIC)

target_sources(starlib PRIVATE
        general/string.cpp
        math/easing.cpp
        math/frustum.cpp
        types/block_allocator.cpp
        types/bitvector.cpp
)

target_sources(starlib PUBLIC FILE_SET HEADERS BASE_DIRS ${H_SOURCES_ROOT} FILES
        general/colors.hpp
        general/string.hpp
        math/glm.hpp
        math/math.hpp
        math/easing.hpp
        math/interpolatable.hpp
        math/frustum.hpp
        types/starlib_stdint.hpp
        types/polymorphic.hpp
        types/block_allocator.hpp
        types/index_allocator.hpp
        types/bitvector.hpp
        types/palette_vector.hpp
)

target_include_directories(starlib PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/starlib>
)

target_include_directories(starlib PUBLIC ${GLM_INCLUDE_DIRS})

target_link_libraries(starlib PUBLIC glm::glm)
target_link_libraries(starlib PRIVATE TracyClient)

target_compile_options(starlib PUBLIC /EHsc -Wno-microsoft-extra-qualification)
set_target_properties(starlib PROPERTIES LINKER_LANGUAGE CXX)

