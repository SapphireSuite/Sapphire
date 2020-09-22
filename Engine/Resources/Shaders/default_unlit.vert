#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 0) uniform StaticUniformBuffer
{
    mat4 view;
    mat4 proj;
} subo;

layout(binding = 1) uniform ObjectUniformBuffer
{
    mat4 model;
} oubo;



// In.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexture;


// Out.
layout(location = 0) out vec2 outTexture;


// Code
void main()
{
    gl_Position = subo.proj * inverse(subo.view) * oubo.model * vec4(inPosition, 1.0);

    outTexture = inTexture;
}