cmake_minimum_required(VERSION 3.16)
project(3d_rendering)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
set(LOCAL_LIB_DIR "/home/rodrigo/Documents/offline_libraries")
if(EXISTS "${LOCAL_LIB_DIR}/glfw")
message(STATUS "Found GLFW locally at ${LOCAL_LIB_DIR}/glfw. Using local copy.")
FetchContent_Declare(glfw
        SOURCE_DIR "${LOCAL_LIB_DIR}/glfw"
)
else()
message(STATUS "GLFW not found locally. Downloading from GitHub...")
FetchContent_Declare(glfw
GIT_REPOSITORY https://github.com/glfw/glfw.git
GIT_TAG 3.4
)
endif()

# --- 3. Fetch GLM ---
if(EXISTS "${LOCAL_LIB_DIR}/glm")
message(STATUS "Found GLM locally at ${LOCAL_LIB_DIR}/glm. Using local copy.")
FetchContent_Declare(glm
SOURCE_DIR "${LOCAL_LIB_DIR}/glm"
)
else()
message(STATUS "GLM not found locally. Downloading from GitHub...")
FetchContent_Declare(glm
GIT_REPOSITORY https://github.com/g-truc/glm.git
GIT_TAG 1.0.1
)
endif()
file(GLOB_RECURSE SOURCES "src/*.cpp" "external/glad/src/*.c")

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})
FetchContent_MakeAvailable(glfw glm)
# --- 4. Includes & Linking ---
target_include_directories(${PROJECT_NAME} PRIVATE
        include
        external/glad/include
)

find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        glm::glm
        ${OPENGL_LIBRARIES}
        ${CMAKE_DL_LIBS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
)

# Get a list of all shader files
file(GLOB SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*")
