#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform sampler2D texSamplers[1];


// In.
layout(location = 0) in DataBlock
{
	vec2 texture;
} fsIn;


// Out.
layout(location = 0) out vec4 outColor;


// Code.
void main()
{
    outColor = texture(texSamplers[0], fsIn.texture);
}