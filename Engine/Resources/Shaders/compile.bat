
@echo off

::for %%x in (%*) do "D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" %%x -o vert.spv

"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" default.vert -o ../../../Bin/Shaders/default_vert.spv
"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" default.frag -o ../../../Bin/Shaders/default_frag.spv

"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" gizmo.vert -o ../../../Bin/Shaders/gizmo_vert.spv
"D:/Program Files/VulkanSDK/1.2.148.1/Bin32/glslc.exe" gizmo.frag -o ../../../Bin/Shaders/gizmo_frag.spv

pause