add_library(starlib STATIC)

target_sources(starlib PRIVATE
    math/easing.cpp
)

target_sources(starlib PUBLIC FILE_SET HEADERS BASE_DIRS ${H_SOURCES_ROOT} FILES
        math/glm.hpp
        math/math.hpp
        math/easing.hpp
        math/interpolatable.hpp
        types/sized_numerics.hpp
)

target_include_directories(starlib PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/starlib>
)

target_include_directories(starlib PUBLIC ${GLM_INCLUDE_DIRS})

target_link_libraries(starlib PUBLIC glm::glm)

target_compile_options(starlib PUBLIC /EHsc -Wno-microsoft-extra-qualification)
set_target_properties(starlib PROPERTIES LINKER_LANGUAGE CXX)

