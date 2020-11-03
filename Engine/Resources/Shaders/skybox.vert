// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
// CameraUBO: shared across shader instances.
struct Camera
{
	// Camera projection matrix.
    mat4 proj;
    
	// Camera inverse transformation matrix.
    mat4 viewInv;

    // Camera position.
    vec3 viewPosition;
};

layout(binding = 0) buffer CameraUniformBuffer
{
    Camera cameras[];
} cameraUBO;


// In.
layout(location = 0) in vec3 inPosition;


// Out.
layout(location = 0) out DataBlock
{
	vec3 texture;
} vsOut;


// Constants.
layout(push_constant) uniform PushConstant
{
    int camIndex;
} pConst;


// Code
void main()
{
    // Position.
    mat4 staticView = mat4(mat3(cameraUBO.cameras[pConst.camIndex].viewInv)); // Remove camera translation.
    gl_Position = cameraUBO.cameras[pConst.camIndex].proj * staticView * vec4(inPosition, 1.0);
	
    gl_Position = gl_Position.xyww; // Depth Test trick to draw faster: only if no object in front of it


    // Texture.
	vsOut.texture = inPosition;
}