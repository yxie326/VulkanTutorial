cmake_minimum_required(VERSION 3.5)
project(VulkanTutorial)

include_directories(
    "C:/VulkanSDK/1.3.250.0/Include"
    "C:/glfw-3.3.8.bin.WIN64/include"
    "C:/glm"
    "D:/libs/TinyObjectLoader"
    "./inc"
)

link_directories(
    "C:/VulkanSDK/1.3.250.0/Lib"
    "C:/glfw-3.3.8.bin.WIN64/lib-mingw-w64"
)

# Source files
file(GLOB SOURCES src/*.cpp)

# Set the output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

# Create the target executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against Vulkan and GLFW libraries
target_link_libraries(${PROJECT_NAME} vulkan-1 glfw3)
