// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450

#extension GL_ARB_separate_shader_objects : enable

// Uniform.
layout(binding = 0) uniform LightViewUniformBuffer
{
	// Light position and projection matrix.
    mat4 viewProj;
} lView;

// Object UBO: specific to this shader instance.
layout(binding = 1) uniform ObjectUniformBuffer
{
	// Model transformation matrix.
    mat4 modelMat;
} objectUBO;


 // In.
layout(location = 0) in vec3 inPosition;


// Code.
void main()
{
   gl_Position = lView.viewProj * objectUBO.modelMat * vec4(inPosition, 1.0);
}