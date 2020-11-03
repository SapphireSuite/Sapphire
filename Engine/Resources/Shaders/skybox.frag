// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable

// Uniform.
layout(binding = 2) uniform samplerCube texSampler;

// In.
layout(location = 0) in DataBlock
{
	vec3 texture;
} fsIn;


// Out.
layout(location = 0) out vec4 outColor;


// Code.
void main()
{
	outColor = texture(texSampler, fsIn.texture);
}