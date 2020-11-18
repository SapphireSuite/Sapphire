// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable

// In.
layout(location = 0) in vec3 inPosition;


// Out.
layout(location = 0) out DataBlock
{
	vec3 position;
} vsOut;


// Constants.
const mat4 proj = mat4(0.0);
const mat4 view = mat4(0.0);


void main()
{
    vsOut.position = inPosition;  
    gl_Position =  proj * view * vec4(inPosition, 1.0);
}