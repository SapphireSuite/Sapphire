#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform sampler2D texSamplers[3];



// Constants.
layout(constant_id = 0) const int alphaModel = 0;
layout(constant_id = 1) const int illumModel = 0;


// In.
layout(location = 0) in DataBlock
{
    vec3 position;
	vec2 texture;
} fsIn;


// Out.
layout(location = 0) out vec4 outColor;


// Code.
void main()
{
	outColor = texture(texSamplers[0], fsIn.texture);

	// Alpha model.

	// Opaque.
	if(alphaModel == 0)
		outColor.a = 1.0;
	else if(outColor.a < 0.001)
		discard;
}