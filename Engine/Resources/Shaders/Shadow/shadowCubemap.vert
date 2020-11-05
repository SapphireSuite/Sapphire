// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450

#extension GL_ARB_separate_shader_objects : enable

// Uniform.
layout(binding = 0) uniform LightViewUniformBuffer
{
	// Light projection matrix.
    mat4 proj;

    // Light position.
    vec3 position;

    float farPlane;
} lView;


struct ModelUBO
{
	// Model transformation matrix.
    mat4 modelMat;
};

// Object UBO: specific to this shader instance.
layout(binding = 1) buffer ObjectUniformBuffer
{
    ModelUBO[] models;
} objectUBO;


// In.
layout(location = 0) in vec3 inPosition;


// Code.
void main()
{
    mat4 viewProj = lView.proj;
    
    // Add Position.
    viewProj[0][3] = lView.position.x;
    viewProj[1][3] = lView.position.y;
    viewProj[2][3] = lView.position.z;

    gl_Position = viewProj * objectUBO.models[gl_InstanceIndex].modelMat * vec4(inPosition, 1.0);
}