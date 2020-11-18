// Copyright 2020 Sapphire development team. All Rights Reserved.

#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 0) uniform sampler2D equirectangularMap;


// In.
layout(location = 0) in DataBlock
{
	vec3 position;
} fsIn;


// Out.
layout(location = 0) out vec4 outRightColor;
layout(location = 1) out vec4 outLeftColor;
layout(location = 2) out vec4 outUpColor;
layout(location = 3) out vec4 outDownColor;
layout(location = 4) out vec4 outFrontColor;
layout(location = 5) out vec4 outBackColor;


// Constants.
const vec2 invAtan = vec2(0.1591, 0.3183);


// Code.
vec2 SampleSphericalMap(vec3 _pos)
{
    vec2 uv = vec2(atan(_pos.z, _pos.x), asin(_pos.y));
    uv *= invAtan;
    uv += 0.5;
    
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(fsIn.position));
    
    outRightColor = vec4(texture(equirectangularMap, uv).rgb, 1.0);
}
