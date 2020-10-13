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
	vec2 texture;
} vsOut;


// Code
void main()
{
    // always facing camera.
    mat4 viewRot = inverse(sUBO.viewInv);
    viewRot[3].xyz = vec3(0,0,0);

    // Position.
    gl_Position = sUBO.proj * sUBO.viewInv *  oUBO.model * viewRot * vec4(inPosition, 1.0);

    // Texture
    vsOut.texture = inTexture;
}