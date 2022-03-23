cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ${FILE} -static-libgcc -static-libstdc++")
project( renderer_alpha)

find_package(Vulkan REQUIRED)

find_package(OpenGL REQUIRED)

if( UNIX )
    find_package(glfw3 3.3)
    set( GLFW glfw) 
elseif( WIN32 )
    set( GLFW${CMAKE_SOURCE_DIR}/GLFW/mingw/libglfw3.a )
endif()

add_executable(${PROJECT_NAME} main.cpp)
include_directories( ${Vulkan_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${GLFW} ${Vulkan_LIBRARIES} OpenGL::GL)