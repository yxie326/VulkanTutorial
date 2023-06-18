cmake_minimum_required(VERSION 3.5)
project(VulkanTutorial)

include_directories(
    "C:/VulkanSDK/1.3.250.0/Include"
    "C:/glfw-3.3.8.bin.WIN64/include"
    "C:/glm"
    "./inc"
)

link_directories(
    "C:/VulkanSDK/1.3.250.0/Lib"
    "C:/glfw-3.3.8.bin.WIN64/lib-mingw-w64"
)

# Source files
file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# Link against Vulkan and GLFW libraries
target_link_libraries(${PROJECT_NAME} vulkan-1 glfw3)
