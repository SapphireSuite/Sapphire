#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
// Static UBO: common to all shader instances.
layout(binding = 0) uniform StaticUniformBuffer
{
	// Camera inverse transformation matrix.
    mat4 viewInv;
    
	// Camera projection matrix.
    mat4 proj;

} sUBO;

// Object UBO: specific to this shader instance.
layout(binding = 1) uniform ObjectUniformBuffer
{
	// Model transformation matrix.
    mat4 model;

	// Material UV tilling.
    float uvTilling;

	// Material UV offset.
    float uvOffset;

} oUBO;


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
    gl_Position = sUBO.proj * sUBO.viewInv * oUBO.model * vec4(inPosition, 1.0);


    // Normal
    vsOut.normal = mat3(oUBO.model) * inNormal;


    // Texture
    vsOut.texture = inTexture * oUBO.uvTilling + oUBO.uvOffset;


    // Camera's vectors.
	vsOut.camPosition = sUBO.viewInv[3].xyz;
}