add_library(starlib STATIC)

target_sources(starlib PRIVATE
        async/async.cpp
        async/threadpool.cpp
        async/threadpool_task.cpp
        general/gameloop.cpp
        general/logger.cpp
        general/clock.cpp
        general/time.cpp
        general/resource_manager.cpp
        math/easing.cpp
        utility/bitvector.cpp
        utility/block_allocator.cpp
        utility/string.cpp
        input/virtual_mouse.cpp
        input/virtual_keyboard.cpp
        input/virtual_controller.cpp
)

target_sources(starlib PUBLIC FILE_SET HEADERS BASE_DIRS ${H_SOURCES_ROOT} FILES
        async/async.hpp
        async/threadpool.hpp
        async/threadpool_task.hpp
        general/gameloop.hpp
        general/graphics.hpp
        general/logger.hpp
        general/stdint.hpp
        general/status.hpp
        general/clock.hpp
        general/time.hpp
        general/resource_manager.hpp
        math/easing.hpp
        math/frustum.hpp
        math/glm.hpp
        math/interpolatable.hpp
        math/random.hpp
        utility/bitvector.hpp
        utility/block_allocator.hpp
        utility/colors.hpp
        utility/heap_value.hpp
        utility/index_allocator.hpp
        utility/palette_vector.hpp
        utility/polymorphic.hpp
        utility/string.hpp
        input/virtual_mouse.hpp
        input/virtual_keyboard.hpp
        input/virtual_controller.hpp
)

target_include_directories(starlib PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/starlib>
)

target_include_directories(starlib PUBLIC ${GLM_INCLUDE_DIRS})
target_include_directories(starlib PUBLIC ${PCG_INCLUDE_DIRS})

target_link_libraries(starlib PRIVATE nanotime)
target_link_libraries(starlib PUBLIC glm::glm)
target_link_libraries(starlib PRIVATE TracyClient)
target_link_libraries(starlib PRIVATE stblib)

target_compile_options(starlib PUBLIC /EHsc -Wno-microsoft-extra-qualification)
set_target_properties(starlib PROPERTIES LINKER_LANGUAGE CXX)

