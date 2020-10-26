#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
// Uniform.
// CameraUBO: shared across shader instances.
struct Camera
{
	// Camera projection matrix.
    mat4 proj;
    
	// Camera inverse transformation matrix.
    mat4 viewInv;

    // Camera position.
    vec3 viewPosition;
};

layout(binding = 0) buffer CameraUniformBuffer
{
    Camera cameras[];
} cameraUBO;


// Object UBO: specific to this shader instance.
layout(binding = 1) uniform ObjectUniformBuffer
{
	// Model transformation matrix.
    mat4 modelMat;

	// Material UV tilling.
    float uvTilling;

	// Material UV offset.
    float uvOffset;

} objectUBO;


// In.
layout(location = 0) in vec3 inPosition;
layout(location = 3) in vec2 inTexture;


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
    vec4 modelPosition = objectUBO.modelMat * vec4(inPosition, 1.0);

    vsOut.position = modelPosition.xyz / modelPosition.w;
    gl_Position = cameraUBO.cameras[pConst.camIndex].proj * cameraUBO.cameras[pConst.camIndex].viewInv * modelPosition;


    // Texture
    vsOut.texture = inTexture * objectUBO.uvTilling + objectUBO.uvOffset;
}