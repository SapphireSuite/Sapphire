// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
/*
*	0. albedo
*	1. normalMap
*	2. heightMap
*	3. metallicMap
*	4. roughMap
*	5. ambiantOcclusion.
*/
layout(binding = 2) uniform sampler2D texSamplers[6];


// In.
layout(location = 0) in DataBlock
{
    vec3 position;
	vec3 normal;
	vec3 tangent;
	vec2 texture;
} fsIn;


// Out.
layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outAlbedo;
layout(location = 3) out vec4 outPBR;


// Code.
void main()
{
	// === Position ===
	outPosition = vec4(fsIn.position, 1.0);
	
	// === Normal ===
	//if(hasNormalMap)
	//{
		// Normal mapping.
		vec3 N = normalize(fsIn.normal);
		vec3 T = normalize(fsIn.tangent);
		vec3 B = cross (N, T);
		mat3 TBN = mat3(T, B, N);
		outNormal = vec4(normalize(TBN * (texture(texSamplers[1], fsIn.texture).rgb * 2.0 - 1.0)), 1.0);
	//}
	//else
	//	outNormal = vec4(fsIn.normal, 1.0);

	// === Albedo ===
	outAlbedo = texture(texSamplers[0], fsIn.texture);
	
	// === PBR ===
	outPBR.x = texture(texSamplers[3], fsIn.texture).r; // Metallic.
	outPBR.y = texture(texSamplers[4], fsIn.texture).r; // Roughness.
	outPBR.z = texture(texSamplers[5], fsIn.texture).r; // Ambiant Occlusion.
	outPBR.w = texture(texSamplers[3], fsIn.texture).r; // HeightMap.
}