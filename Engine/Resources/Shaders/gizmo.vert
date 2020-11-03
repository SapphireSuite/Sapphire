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
} objectUBO;


// In.
layout(location = 0) in vec3 inPosition;
layout(location = 3) in vec2 inTexture;


// Out.
layout(location = 0) out DataBlock
{
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
    // always facing camera.
    mat4 viewRot = inverse(cameraUBO.cameras[pConst.camIndex].viewInv);
    viewRot[3].xyz = vec3(0,0,0);

    // Position.
    gl_Position = cameraUBO.cameras[pConst.camIndex].proj * cameraUBO.cameras[pConst.camIndex].viewInv *  objectUBO.modelMat * viewRot * vec4(inPosition, 1.0);

    // Texture
    vsOut.texture = inTexture;
}