// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450

#extension GL_ARB_separate_shader_objects : enable

// Uniform.
layout(binding = 0) uniform LightViewUniformBuffer
{
	// Light transform and projection matrix.
    mat4 viewProj;

    // Light position.
    vec3 position;

    float farPlane;
} lView;

// In.
layout(location = 0) in vec4 inPosition;


// Code.
void main()
{
    // Get distance between fragment and light source.
    float lightDistance = length(inPosition.xyz - lView.position);
    
    // Map to [0;1] range by dividing by far_plane.
    lightDistance = lightDistance / lView.farPlane;
    
    // Write this as modified depth.
    gl_FragDepth = lightDistance;
}