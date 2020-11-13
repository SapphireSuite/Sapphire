#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform sampler2D texSampler;


// In.
layout(location = 0) in DataBlock
{
    vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texture;
} fsIn;


// Out.
layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec3 outAlbedo;
layout(location = 3) out vec3 outPBR;


// Code.
void main()
{
	outPosition = vec4(fsIn.position, 1.0);
	outNormal = vec4(fsIn.normal, 1.0);
	outAlbedo = texture(texSampler, fsIn.texture).xyz;
	outPBR = vec3(1.0);
}