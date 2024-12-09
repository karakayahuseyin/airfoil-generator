cmake_minimum_required(VERSION 3.5.0)
project(airfoil-generator)

set(SRC_DIR 
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

set(INCLUDE_DIRECTORIES 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(SOURCES
    "${SRC_DIR}/main.cpp"
    "${INCLUDE_DIRECTORIES}/glad/glad.c"
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_library(imgui STATIC
    ${INCLUDE_DIRECTORIES}/imgui/imgui.cpp
    ${INCLUDE_DIRECTORIES}/imgui/imgui_draw.cpp
    ${INCLUDE_DIRECTORIES}/imgui/imgui_demo.cpp
    ${INCLUDE_DIRECTORIES}/imgui/imgui_tables.cpp
    ${INCLUDE_DIRECTORIES}/imgui/imgui_widgets.cpp
)

target_include_directories(imgui PUBLIC ${INCLUDE_DIRECTORIES}/imgui)

add_library(imgui_backend STATIC
    ${INCLUDE_DIRECTORIES}/imgui/backends/imgui_impl_glfw.cpp
    ${INCLUDE_DIRECTORIES}/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui_backend PUBLIC
    ${INCLUDE_DIRECTORIES}/imgui
    ${INCLUDE_DIRECTORIES}/imgui/backends
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLU REQUIRED)

target_link_libraries(imgui_backend imgui glfw OpenGL::GL)
target_link_libraries(${PROJECT_NAME} imgui_backend GLU imgui glfw OpenGL::GL m GLEW::GLEW)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRECTORIES} GLU glm::glm GLEW::GLEW)