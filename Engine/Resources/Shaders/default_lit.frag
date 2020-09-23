#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout (binding = 2) uniform PointLight
{
	vec3 position;

	vec3 color;
	
	float ambiant;
	float diffuse;
	float specular;
    
    float shininess;
} pLights[1];

layout(binding = 3) uniform sampler2D texSamplers[4];


// Constants.
layout(push_constant) uniform MaterialConstants
{
	vec3 Ka;
    vec3 Kd;
	vec3 Ks;
	vec3 Ke;
	vec3 Tf;

	float d;
	float Ni;
	float Ns;
} matConsts;

const float PI = 3.14159265359;

const float kc = 0.01; //constant
const float kl = 0.2; //linear
const float kq = 0.2; //quadratic

// In.
layout(location = 0) in DataBlock
{
    vec3 position;
	vec3 normal;
	vec2 texture;

	vec3 camPosition;
} fsIn;


// Out.
layout(location = 0) out vec4 outColor;


// Code.
void main()
{
	// Normal vector.
	vec3 vNorm = normalize(fsIn.normal);

	// Object to light direction.
	vec3 vLight = normalize(pLights[0].position - fsIn.position);

	// Object to camera direction.
	vec3 vCam = normalize(fsIn.position - fsIn.camPosition);

	// Halfway vector.
	vec3 vHalf = normalize(vLight + vCam);

	// Normal facing light.
	float facing = dot(vNorm, vLight) > 0.0 ? 1.0 : 0.0;

	vec3 ambiant = pLights[0].color *  pLights[0].ambiant * matConsts.Ka;
	vec3 diffuse = pLights[0].color * pLights[0].diffuse * matConsts.Kd * max(dot(vNorm, vLight), 0.0) * texture(texSamplers[0], fsIn.texture).xyz;
	vec3 specular = pLights[0].color * pow(max(dot(vNorm, vHalf), 0.0), pLights[0].shininess) * pLights[0].specular * matConsts.Ks * facing;  

	float attenuation = 1.0 / (kc + kl * length(vLight) + kq * dot(vLight, vLight));

	outColor.xyz = ambiant + attenuation * (diffuse + specular);
	outColor.a = 1.0;
}