cmake_minimum_required(VERSION 3.12)
project("POGL" CXX)

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "RELEASE")
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -fno-inline -g3")

set(SRC
    "src/main.cc"
    "src/gl/shader.cc"
    "src/gl/program.cc"
    "src/noise/perlin3d.cc"
    "src/gl/vao.cc"
    "src/particles/particle.cc"
    "src/particles/fire.cc"
    "src/particles/sun.cc"
    "src/engine.cc"
    "src/camera.cc")

set(LIBRARIES GL GLU glut GLEW)

set(INCLUDE_DIR "src")

add_executable(pogl)
target_sources(pogl PRIVATE ${SRC})
target_include_directories(pogl PRIVATE ${INCLUDE_DIR} ${GLM_INCLUDE_DIRS}
                           ${GLFW_LIBRARIES})

target_link_libraries(pogl PRIVATE ${LIBRARIES} glfw)
