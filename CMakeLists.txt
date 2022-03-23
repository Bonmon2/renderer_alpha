cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${FILE} -static-libgcc -static-libstdc++")
project( renderer_alpha)

find_package(Vulkan REQUIRED)

set( renderer renderer/renderer.cpp renderer/debug/debug.cpp)

set( window window/window.cpp )

set( incDirs ${Vulkan_INCLUDE_DIRS} )

if( UNIX )
    find_package(glfw3 3.3)
    set( GLFW glfw)

elseif( WIN32 )
    set( GLFW ${CMAKE_SOURCE_DIR}/GLFW/mingw/libglfw3.a )
    set( incDirs ${incDirs} GLFW/include )
    
endif()
message(${incDirs})

add_executable( ${PROJECT_NAME} main.cpp ${window} ${renderer} )
include_directories( ${incDirs} )

target_link_libraries( ${PROJECT_NAME} ${Vulkan_LIBRARIES} ${GLFW})
