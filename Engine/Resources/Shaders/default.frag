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
	
	float shininess;

	// Transparency.
	float alpha;

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
void BlinnPhongIllumination();
void PBRIllumination();

float Fresnel(float _f0, float _cosTheta);

void main()
{
	// Illumination model.
	if(illumModel == 0)				// Unlit.
		outColor.xyz = texture(texSamplers[0], fsIn.texture).xyz;
	else if(illumModel == 1)
		BlinnPhongIllumination();	// Lit: simple Blinn-Phong implementation.
	else if(illumModel == 2)
		PBRIllumination();			// Lit: Physically based using BRDF.


	// Alpha model.
	if(alphaModel == 0)				// Opaque.
		outColor.a = 1.0;
	else if(alphaModel == 1)		// Allows transparency.
		outColor.a = matConsts.alpha;
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


	float cosTheta = dot(vNorm, vLight);

	// Blinn-Phong variant. Phong formula is: dot(vNorm, vCam)
	float cosAlpha = max(dot(vNorm, vHalf), 0.0);


	// Normal facing light.
	float facing = cosTheta > 0.0 ? 1.0 : 0.0;
	

	// Ambiant component.
	vec3 Ra = pLights[0].color * pLights[0].ambiant * matConsts.ka;
	
	// Diffuse component.
	vec3 Rd = pLights[0].color * pLights[0].diffuse * matConsts.kd * texture(texSamplers[0], fsIn.texture).xyz;

	// Specular component.
	vec3 Rs = pLights[0].color * pLights[0].specular * matConsts.ks;


	vec3 diffuse = Rd * max(cosTheta, 0.0);
	vec3 specular = Rs * pow(cosAlpha, matConsts.shininess) * facing;  
	
	float attenuation = 1.0 / (kc + kl * length(vLight) + kq * dot(vLight, vLight));
	
	outColor.xyz = Ra + attenuation * (diffuse + specular);
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


	float cosTheta = dot(vNorm, vLight);

	// Blinn-Phong variant. Phong formula is: dot(vNorm, vCam)
	float cosAlpha = max(dot(vNorm, vHalf), 0.0);


	/* 
	*	Normalization factor: Gotanda approximation.
	*	Source: http://research.tri-ace.com/Data/course_note_practical_implementation_at_triace.pdf
	*/
	//float normFactor = 0.0397436 * matConsts.shininess + 0.0856832;
	float normFactor = (matConsts.shininess + 2) / (4 /** PI*/ * (2 - pow(2, -matConsts.shininess / 2)));

	// Geometric factor: Neumann "Albedo pumped-up".
	float G = 1.0 / max(cosTheta, dot(vNorm, vCam));


	// Ambiant component.
	vec3 Ra = pLights[0].color * pLights[0].ambiant * matConsts.ka;
	
	// Diffuse component.
	vec3 Rd = pLights[0].color * pLights[0].diffuse * matConsts.kd * texture(texSamplers[0], fsIn.texture).xyz;

	// Specular component.
	vec3 Rs = pLights[0].color * pLights[0].specular * matConsts.ks;


	vec3 diffuseBRDF = Rd/* / PI*/;
	vec3 specularBRDF = Rs * normFactor * Fresnel(0.8, dot(vCam, vHalf)) * G * pow(cosAlpha, matConsts.shininess);
	
	outColor.xyz = Ra + (diffuseBRDF + specularBRDF) * cosTheta;
}


float Fresnel(float _f0, float _cosTheta)
{
	// f0: Material reflectance at 0 degree.

	// Schlick�s approximation.
	return _f0 + (1 - _f0) * pow(1 - _cosTheta, 5);
}