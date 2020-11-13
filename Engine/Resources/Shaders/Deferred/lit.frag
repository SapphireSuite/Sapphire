#version 450
#extension GL_KHR_vulkan_glsl : enable
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout (set = 0, input_attachment_index = 0, binding = 0) uniform subpassInput samplerPosition;
layout (set = 0, input_attachment_index = 1, binding = 1) uniform subpassInput samplerNormal;
layout (set = 0, input_attachment_index = 2, binding = 2) uniform subpassInput samplerAlbedo;
layout (set = 0, input_attachment_index = 3, binding = 3) uniform subpassInput samplerPBR;


// In.
layout(location = 0) in vec2 inSamplerUV;


// Out.
layout(location = 0) out vec4 outColor;


// Code.
void main()
{
	outColor = subpassLoad(samplerAlbedo);

	if(outColor.a < 0.001)
		discard;
}