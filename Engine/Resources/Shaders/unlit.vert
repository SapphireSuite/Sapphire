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
    mat4 modelMat;

	// Material UV tilling.
    float uvTilling;

	// Material UV offset.
    float uvOffset;

} oUBO;


// In.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexture;


// Out.
layout(location = 0) out DataBlock
{
    vec3 position;
	vec2 texture;
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
    vec4 modelPosition = oUBO.modelMat * vec4(inPosition, 1.0);

    vsOut.position = modelPosition.xyz / modelPosition.w;
    gl_Position = camUBO[pConst.camIndex].proj * camUBO[pConst.camIndex].viewInv * modelPosition;


    // Texture
    vsOut.texture = inTexture * oUBO.uvTilling + oUBO.uvOffset;
}