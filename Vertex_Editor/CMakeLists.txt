cmake_minimum_required(VERSION 3.16)
project(VertexEditor)

set(CMAKE_CXX_STANDARD 20)

# Set your shared includes
set(SHARED_INCLUDE_DIR "OpenGL_include")
SET(HEADER_DIR "../headers")
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME}
        main.cpp
        ${SOURCES}
        "${SHARED_INCLUDE_DIR}/glad/src/glad.c"
        src/CameraHandler.cpp
        include/Mesh.h

        OpenGL_include/ShaderHandler.h
        src/Mesh.cpp
        src/MouseHandling.cpp
        include/MouseHandling.h
        src/MainWindow.cpp
        include/MainWindow.h
)
target_include_directories(${PROJECT_NAME} PRIVATE
        include
        ${SHARED_INCLUDE_DIR}
        "${SHARED_INCLUDE_DIR}/glad/include"
        HEADER_DIR
)

# Find ALL your global libraries|
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# Link them
target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        assimp::assimp
        glm::glm
        ${OPENGL_LIBRARIES}
        ${CMAKE_DL_LIBS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
        HEADER_DIR="../include"
)