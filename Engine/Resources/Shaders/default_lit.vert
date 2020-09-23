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


layout (binding = 2) uniform PointLight
{
    vec3 position;

	vec3 color;
	
	float ambiant;
	float diffuse;
	float specular;
    
    float shininess;
} pLights[1];

// In.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexture;


// Out.
layout(location = 0) out DataBlock
{
    vec3 position;
	vec3 normal;
	vec2 texture;

	vec3 camPosition;
} vsOut;


// Code
void main()
{
    // Position.
    vsOut.position = mat3(oUBO.model) * inPosition;
    gl_Position = sUBO.proj * inverse(sUBO.view) * vec4(vsOut.position, 1.0);


    // Normal
    vsOut.normal = mat3(oUBO.model) * inNormal;


    // Texture
    vsOut.texture = inTexture;
    //out.texture = inTexture * tiling + offset;


    // Vector camera.
	vsOut.camPosition = sUBO.view[3].xyz;
}