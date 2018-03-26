cmake_minimum_required(VERSION 3.5)
project(HumanGl)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -g")
find_package(OpenGL REQUIRED)

#Handle GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(third-party/glfw)
include_directories(third-party/glfw/include)
include_directories(third-party/glad)
include_directories(third-party/glm)
include_directories(third-party)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(SOURCE_FILES
        third-party/glad/glad.c
		src/main.cpp 
        src/env.cpp
        src/Matrix.cpp
        src/Bone.cpp
        src/shader.cpp
		src/Renderer.cpp
		src/Charactere.cpp
		src/Vec3.cpp 
		src/Camera.cpp
		)
add_executable(HumanGl ${SOURCE_FILES})

#Link GLFW and OpenCL
#target_link_libraries(HumanGl ${GLEW_LIBRARY})
target_link_libraries(HumanGl glfw ${GLFW_LIBRARIES})

