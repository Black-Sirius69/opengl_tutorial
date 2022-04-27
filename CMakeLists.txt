cmake_minimum_required(VERSION 3.22)
project(opengl_tutorial)

set(CMAKE_CXX_STANDARD 20)
link_directories("${CMAKE_SOURCE_DIR}/libraries/glfw/lib-vc2022 ")
link_directories("${CMAKE_SOURCE_DIR}/libraries/glew/lib/Release/x64")
find_package(OpenGL REQUIRED)
include_directories("${CMAKE_SOURCE_DIR}/libraries/glfw/include")
include_directories("${CMAKE_SOURCE_DIR}/libraries/glew/include")
add_compile_definitions(GLEW_STATIC)

add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shaders
        ${CMAKE_BINARY_DIR}/shaders
        COMMENT "copying shaders"
        )

add_executable(opengl_tutorial main.cpp)
add_dependencies(opengl_tutorial copy_resources)

target_link_libraries(opengl_tutorial ${OPENGL_LIBRARIES} glfw3 glew32s)
