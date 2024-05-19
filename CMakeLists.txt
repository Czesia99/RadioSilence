cmake_minimum_required(VERSION 3.22.1)

set(PROJECT_RADIOSILENCE "radio_silence")

project(${PROJECT_RADIOSILENCE} VERSION 0.1.0)

#cmake_policy(SET CMP0072 NEW)
#set(CMAKE_CXX_STANDARD 11.4.0)

find_package(OpenGL REQUIRED)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(SRC ./src)
set(MYGL ./src/mygl)
set(ASSIMP_DIR ./include/assimp)

# set(TEXTURES ./textures)
# set(SHADERS ./shaders)

# Add Assimp library
add_subdirectory(include/assimp)

set(SRC_FILES
    ${MYGL}/glad.c
    ${MYGL}/shape.cpp
    ${MYGL}/model.cpp
    ${SRC}/main.cpp
    ${SRC}/game_scene.cpp
    ${SRC}/menu_scene.cpp
    ${SRC}/credits_scene.cpp
    ${SRC}/texture.cpp
)

set(LINK_LIB
    glfw
    OpenGL::GL
    assimp
)
# include_directories(${SHADERS})
include_directories(${INCLUDE_DIR})
add_executable(${PROJECT_RADIOSILENCE} ${SRC_FILES})
target_link_libraries(${PROJECT_RADIOSILENCE} ${LINK_LIB})