#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
// CameraUBO: shared across shader instances.
layout(binding = 0) uniform CameraUniformBuffer
{
	// Camera inverse transformation matrix.
    mat4 viewInv;
    
	// Camera projection matrix.
    mat4 proj;

} camUBO[5];

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
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec2 inTexture;


// Out.
layout(location = 0) out DataBlock
{
    vec3 position;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
	vec2 texture;

	vec3 camPosition;
} vsOut;


// Constants.
layout(push_constant) uniform PushConstant
{
    int camIndex;
} pConst;


// Code
void main()
{
    // Position.
    vsOut.position = mat3(oUBO.model) * inPosition;
    gl_Position = camUBO[pConst.camIndex].proj * camUBO[pConst.camIndex].viewInv * oUBO.model * vec4(inPosition, 1.0);


    // Normal
    vsOut.normal = mat3(oUBO.model) * inNormal;


    // Tangent
    vsOut.tangent = mat3(oUBO.model) * inTangent;
    vsOut.bitangent = mat3(oUBO.model) * inBitangent;


    // Texture
    vsOut.texture = inTexture * oUBO.uvTilling + oUBO.uvOffset;


    // Camera's vectors.
	vsOut.camPosition = camUBO[pConst.camIndex].viewInv[3].xyz;
}