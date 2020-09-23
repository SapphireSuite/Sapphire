#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 0) uniform StaticUniformBuffer
{
    mat4 view;
    mat4 proj;
} sUBO;

layout(binding = 1) uniform ObjectUniformBuffer
{
    mat4 model;
} oUBO;



// In.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexture;


// Out.
layout(location = 0) out DataBlock
{
	vec2 texture;
} vsOut;


// Code
void main()
{
    gl_Position = sUBO.proj * inverse(sUBO.view) * oUBO.model * vec4(inPosition, 1.0);

    vsOut.texture = inTexture;
    //outTexture = inTexture * tiling + offset;
}