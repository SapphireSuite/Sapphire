// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable


// Out.
out gl_PerVertex
{
	vec4 gl_Position;
};


// Code
void main()
{
	/*
	*	Convert vertex index to screen coordinates:
	*	0: (0, 0)
	*	1: (1, 0)
	*	2: (0, 1)
	*	3: (1, 1)
	*/
	vec2 outSamplerUV = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);


	gl_Position = vec4(outSamplerUV * 2.0f - 1.0f, 0.0f, 1.0f);
}
