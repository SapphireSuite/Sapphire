
@echo off

::for %%x in (%*) do "D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" %%x -o vert.spv

"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" default_unlit.vert -o ../../../Bin/Shaders/default_unlit_vert.spv
"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" default_unlit.frag -o ../../../Bin/Shaders/default_unlit_frag.spv

"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" default_lit.vert -o ../../../Bin/Shaders/default_lit_vert.spv
"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" default_lit.frag -o ../../../Bin/Shaders/default_lit_frag.spv

pause