// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.

// In.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexture;


// Out.
out gl_PerVertex
{
	vec4 gl_Position;
};


// Code
void main()
{
	vec2 outSamplerUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);
	gl_Position = vec4(outSamplerUV * 2.0f - 1.0f, 0.0f, 1.0f);
}