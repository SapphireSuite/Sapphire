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

	vec3 viewPosition;
} fsIn;


// Out.
layout (location = 0) out vec4 outColor;
layout (location = 1) out vec4 outPosition;
layout (location = 2) out vec4 outNormal;
layout (location = 3) out vec3 outAlbedo;
layout (location = 4) out float outMetallic;
layout (location = 5) out float outRoughness;
layout (location = 6) out float outAO;

const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 256.0f;

float linearDepth(float depth)
{
	float z = depth * 2.0f - 1.0f; 
	return (2.0f * NEAR_PLANE * FAR_PLANE) / (FAR_PLANE + NEAR_PLANE - z * (FAR_PLANE - NEAR_PLANE));	
}

void main()
{
	outColor = vec4(0.0);

	outPosition = vec4(fsIn.position, 1.0);
	outPosition.a = linearDepth(gl_FragCoord.z);

	outNormal =	vec4(1.0);
	if(texture(texSamplers[1], fsIn.texture).rgb != vec3(0))
	{
		// Normal mapping.

		vec3 N = normalize(fsIn.normal);
		vec3 T = normalize(fsIn.tangent);
		vec3 B = cross (N, T);
		mat3 TBN = mat3(T, B, N);
		outNormal.rgb = normalize(TBN * (texture(texSamplers[1], fsIn.texture).rgb * 2.0 - 1.0));
	}
	else
		outNormal.rgb = normalize(fsIn.normal); // Vertex normal.

	outAlbedo = texture(texSamplers[0], fsIn.texture).xyz;

	outMetallic = texture(texSamplers[3], fsIn.texture).r;

	outRoughness = texture(texSamplers[4], fsIn.texture).r;
	
	outAO = texture(texSamplers[5], fsIn.texture).r;

	// TODO: CLEAN LATER: missing ao map.
	if(outAO == 0.0)
		outAO = 1.0;
}

