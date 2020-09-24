#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform MaterialConstants
{
	// Ambiant constant.
	vec3 Ka;

	// Diffuse constant.
    vec3 Kd;

	// Specular constant.
	vec3 Ks;

	// Emissive constant.
	vec3 Ke;


	vec3 Tf;

	// Transparency.
	float d;

	// Optical density (refraction index).
	float Ni;

	// Specular exponent.
	float Ns;
} matConsts;

layout (binding = 3) uniform PointLight
{
	vec3 position;

	vec3 color;
	
	float ambiant;
	float diffuse;
	float specular;
    
    float shininess;
} pLights[1];

layout(binding = 4) uniform sampler2D texSamplers[4];


// Constants.
const float PI = 3.14159265359;

layout(constant_id = 1) const int illumModel = 1;


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
void BlinnPhongIllumination();
void PBRIllumination();

float Fresnel(float _f0, float _cosTheta);

void main()
{
	if(illumModel == 0)				// Unlit.
		outColor = texture(texSamplers[0], fsIn.texture);
	else if(illumModel == 1)
		BlinnPhongIllumination();	// Lit: simple Blinn-Phong implementation.
	else if(illumModel == 2)
		PBRIllumination();			// Lit: Physically based using BRDF.
}

void BlinnPhongIllumination()
{
	// Attenuation constants.
	const float kc = 0.01;	// Constant.
	const float kl = 0.2;	// Linear.
	const float kq = 0.2;	// Quadratic.


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
	

	// Ambiant component.
	vec3 Ra = pLights[0].color * pLights[0].ambiant * matConsts.Ka;
	
	// Diffuse component.
	vec3 Rd = pLights[0].color * pLights[0].diffuse * matConsts.Kd * texture(texSamplers[0], fsIn.texture).xyz;

	// Specular component.
	vec3 Rs = pLights[0].color * pLights[0].specular * matConsts.Ks;


	vec3 diffuse = Rd * max(dot(vNorm, vLight), 0.0);
	vec3 specular = Rs * pow(max(dot(vNorm, vHalf), 0.0), pLights[0].shininess) * facing;  
	
	float attenuation = 1.0 / (kc + kl * length(vLight) + kq * dot(vLight, vLight));
	
	outColor.xyz = Ra + attenuation * (diffuse + specular);
	outColor.a = 1.0;
}


void PBRIllumination()
{
	// Normal vector.
	vec3 vNorm = normalize(fsIn.normal);

	// Object to light direction.
	vec3 vLight = normalize(pLights[0].position - fsIn.position);

	// Object to camera direction.
	vec3 vCam = normalize(fsIn.position - fsIn.camPosition);

	// Halfway vector.
	vec3 vHalf = normalize(vLight + vCam);


	/* 
	*	Normalization factor: Gotanda approximation.
	*	Source: http://research.tri-ace.com/Data/course_note_practical_implementation_at_triace.pdf
	*/
	//float normFactor = 0.0397436 * pLights[0].shininess + 0.0856832;
	float normFactor = (pLights[0].shininess + 2) / (4 /** PI*/ * (2 - pow(2, -pLights[0].shininess / 2)));

	// Geometric factor: Neumann "Albedo pumped-up".
	float G = 1.0 / max(dot(vNorm, vLight), dot(vNorm, vCam));


	// Ambiant component.
	vec3 Ra = pLights[0].color * pLights[0].ambiant * matConsts.Ka;
	
	// Diffuse component.
	vec3 Rd = pLights[0].color * pLights[0].diffuse * matConsts.Kd * texture(texSamplers[0], fsIn.texture).xyz;

	// Specular component.
	vec3 Rs = pLights[0].color * pLights[0].specular * matConsts.Ks;


	vec3 diffuseBRDF = Rd/* / PI*/;
	vec3 specularBRDF = Rs * normFactor * Fresnel(0.8, dot(vCam, vHalf)) * G * pow(max(dot(vNorm, vHalf), 0.0), pLights[0].shininess);
	
	outColor.xyz = Ra + (diffuseBRDF + specularBRDF) * dot(vNorm, vLight);
	outColor.a = 1.0;
}


float Fresnel(float _f0, float _cosTheta)
{
	// f0: Material reflectance at 0 degree.

	// Schlick’s approximation.
	return _f0 + (1 - _f0) * pow(1 - _cosTheta, 5);
}