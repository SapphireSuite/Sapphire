#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 1) uniform sampler2D texSampler;


// In.
layout(location = 0) in vec2 fragTexUV;


// Out.
layout(location = 0) out vec4 outColor;


// Code.
void main()
{
    outColor = texture(texSampler, fragTexUV);
}