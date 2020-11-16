#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 0) uniform Camera
{
	// Camera projection matrix.
    mat4 proj;
    
	// Camera inverse transformation matrix.
    mat4 viewInv;

    // Camera position.
    vec3 viewPosition;
} cameraUBO;


// In.
layout(location = 0) in vec3 inPosition;


// Out.
layout(location = 0) out DataBlock
{
	vec3 texture;
} vsOut;


// Code
void main()
{
    // Position.
    mat4 staticView = mat4(mat3(cameraUBO.viewInv)); // Remove camera translation.
    gl_Position = cameraUBO.proj * staticView * vec4(inPosition, 1.0);
	
    gl_Position = gl_Position.xyww; // Depth Test trick to draw faster: only if no object in front of it


    // Texture.
	vsOut.texture = inPosition;
}