C:\VulkanSDK\1.3.250.0\Bin\glslc.exe shaders\simple_shader.vert -o shaders\simple_shader.vert.spv
C:\VulkanSDK\1.3.250.0\Bin\glslc.exe shaders\simple_shader.frag -o shaders\simple_shader.frag.spv
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build