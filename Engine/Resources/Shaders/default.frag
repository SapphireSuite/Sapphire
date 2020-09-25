#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform MaterialConstants
{
	// Ambiant constant.
	vec3 ka;

	// Diffuse constant.
    vec3 kd;

	// Specular constant.
	vec3 ks;

	// Emissive constant.
	vec3 ke;

	// Transmission filter.
	vec3 tf;
	
	// Transparency.
	float alpha;

	float shininess;

	float metallic;

	// Optical density (Refractive index).
	float refraction;
} matConsts;

layout (binding = 3) uniform PointLight
{
	vec3 position;

	vec3 color;
	
	float ambiant;
	float diffuse;
	float specular;
} pLights[1];

layout(binding = 4) uniform sampler2D texSamplers[4];


// Constants.
const float PI = 3.14159265359;

layout(constant_id = 1) const int alphaModel = 0;
layout(constant_id = 2) const int illumModel = 1;


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
void ComputeAlpha();
void ComputeIllumination();


void main()
{
	ComputeAlpha();

	ComputeIllumination();
}


void ComputeAlpha()
{
	// Alpha model.
	if(alphaModel == 0)				// Opaque.
		outColor.a = 1.0;
	else if(alphaModel == 1)		// Allows transparency.
		outColor.a = matConsts.alpha;
}


struct IlluminationData
{
	// Normal vector.
	vec3 vNorm;

	// Object to light direction.
	vec3 vLight;

	// Object to camera direction.
	vec3 vCam;

	// Halfway vector.
	vec3 vHalf;


	// Dot(Normal, vLight).
	float cosTheta;

	// Blinn-Phong variant: dot(vNorm, vCam). Phong formula is: Dot(vNorm, vCam).
	float cosAlpha;


	// Ambiant component.
	vec3 Ra;
	
	// Diffuse component.
	vec3 Rd;

	// Specular component.
	vec3 Rs;
};

void PBRIllumination(IlluminationData _data);
void BlinnPhongIllumination(IlluminationData _data);
float Fresnel(float _f0, float _cosTheta);

void ComputeIllumination()
{
	// Illumination model: Unlit
	if(illumModel == 0)
	{
		outColor.xyz = texture(texSamplers[0], fsIn.texture).xyz;
		return;
	}


	// Illumination model: Lit.
	IlluminationData data;

	// Albedo from texture.
	vec3 albedo = texture(texSamplers[0], fsIn.texture).xyz;


	// Normal vector.
	data.vNorm = normalize(fsIn.normal);

	// Object to light direction.
	data.vLight = normalize(pLights[0].position - fsIn.position);

	// Object to camera direction.
	data.vCam = normalize(fsIn.position - fsIn.camPosition);

	// Halfway vector.
	data.vHalf = normalize(data.vLight + data.vCam);


	data.cosTheta = dot(data.vNorm, data.vLight);

	// Blinn-Phong variant. Phong formula is: dot(vNorm, vCam)
	data.cosAlpha = max(dot(data.vNorm, data.vHalf), 0.0);


	// Ambiant component.
	data.Ra = pLights[0].color * pLights[0].ambiant * matConsts.ka;
	
	// Diffuse component.
	data.Rd = pLights[0].color * pLights[0].diffuse * matConsts.kd * mix(albedo, vec3(0.0), matConsts.metallic);

	// Specular component.
	data.Rs = pLights[0].color * pLights[0].specular * matConsts.ks * mix(vec3(1.0), albedo, matConsts.metallic);


	// Illumination model.
	if(illumModel == 1)
		BlinnPhongIllumination(data);	// Simple Blinn-Phong implementation.
	else if(illumModel == 2)
		PBRIllumination(data);			// Physically based using BRDF.
}

void BlinnPhongIllumination(IlluminationData _data)
{
	// Attenuation constants.
	const float kc = 0.01;	// Constant.
	const float kl = 0.2;	// Linear.
	const float kq = 0.2;	// Quadratic.
	

	// Normal facing light.
	float facing = _data.cosTheta > 0.0 ? 1.0 : 0.0;

	vec3 diffuse = _data.Rd * max(_data.cosTheta, 0.0);
	vec3 specular = _data.Rs * pow(_data.cosAlpha, matConsts.shininess) * facing;  
	
	float attenuation = 1.0 / (kc + kl * length(_data.vLight) + kq * dot(_data.vLight, _data.vLight));
	

	//  Output.
	outColor.xyz = _data.Ra + attenuation * (diffuse + specular);
}


void PBRIllumination(IlluminationData _data)
{
	/* 
	*	Normalization factor: Gotanda approximation.
	*	Source: http://research.tri-ace.com/Data/course_note_practical_implementation_at_triace.pdf
	*/
	//float normFactor = 0.0397436 * matConsts.shininess + 0.0856832;
	float normFactor = (matConsts.shininess + 2) / (4 /** PI*/ * (2 - pow(2, -matConsts.shininess / 2)));

	// Geometric factor: Neumann "Albedo pumped-up".
	float G = 1.0 / max(_data.cosTheta, dot(_data.vNorm, _data.vCam));


	vec3 diffuseBRDF = _data.Rd/* / PI*/;
	vec3 specularBRDF = _data.Rs * normFactor * /*Fresnel(0.8, dot(vCam, vHalf)) **/ G * pow(_data.cosAlpha, matConsts.shininess);
	

	//  Output.
	outColor.xyz = _data.Ra + (diffuseBRDF + specularBRDF) * _data.cosTheta;
}


float Fresnel(float _f0, float _cosTheta)
{
	// f0: Material reflectance at 0 degree.

	// Schlick’s approximation.
	return _f0 + (1 - _f0) * pow(1 - _cosTheta, 5);
}