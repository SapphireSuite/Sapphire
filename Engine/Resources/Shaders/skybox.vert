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


// In.
layout(location = 0) in vec3 inPosition;


// Out.
layout(location = 0) out DataBlock
{
	vec3 texture;
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
    mat4 staticView = mat4(mat3(camUBO[pConst.camIndex].viewInv)); // Remove camera translation.
    gl_Position = camUBO[pConst.camIndex].proj * staticView * vec4(inPosition, 1.0);
	
    gl_Position = gl_Position.xyww; // Depth Test trick to draw faster: only if no object in front of it
    gl_Position.z -= 0.0001; // zfight quick fix.


    // Texture.
	vsOut.texture = inPosition;
}