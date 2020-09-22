#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform sampler2D texSampler[4];

layout(push_constant) uniform PushConstants
{
    vec3 Kd;
	vec3 Ka;
	vec3 Ks;
	vec3 Ke;
	vec3 Tf;

	float d;
	float Ni;
	float Ns;
} pConsts;

// In.
layout(location = 0) in vec2 inTexture;


// Out.
layout(location = 0) out vec4 outColor;


// Code.
void main()
{
    outColor = texture(texSampler[0], inTexture);
}