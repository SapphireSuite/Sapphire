#version 450
#extension GL_ARB_separate_shader_objects : enable

// Uniform.
layout(binding = 1) uniform samplerCube texSampler;

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