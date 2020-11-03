// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable


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

    // Normal transformation matrix.
    mat4 normalMat; // TODO: use mat3.

	// Material UV tilling.
    float uvTilling;

	// Material UV offset.
    float uvOffset;

} objectUBO;


// In.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inTexture;


// Out.
layout(location = 0) out DataBlock
{
    vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texture;

	vec3 viewPosition;
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

    
    // Normal & Tangent
    // if uniform scale.
    mat3 modelMat3 = mat3(objectUBO.modelMat);
    vsOut.normal = modelMat3 * inNormal;
    vsOut.tangent = modelMat3 * inTangent;
    // else if non-uniform scale.
    //vsOut.normal = mat3(objectUBO.normalMat) * inNormal; // TODO: remove mat3() (default is mat3).
    //vsOut.tangent = mat3(objectUBO.normalMat) * inTangent; // TODO: remove mat3() (default is mat3).


    // Texture
    vsOut.texture = inTexture * objectUBO.uvTilling + objectUBO.uvOffset;


    // Camera's vectors.
	vsOut.viewPosition = cameraUBO.cameras[pConst.camIndex].viewPosition;
}