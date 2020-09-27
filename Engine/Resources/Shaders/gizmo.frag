#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform sampler2D texSamplers[4];


// Constants.
const float PI = 3.14159265359;


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
	//outColor.xyz = texture(texSamplers[0], fsIn.texture).xyz;
	//outColor.a = 1.0;

	outColor = vec4(1.0, 1.0, 1.0, 1.0);
}