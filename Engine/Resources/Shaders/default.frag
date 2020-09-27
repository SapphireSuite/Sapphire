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


struct Light
{
	vec3 position;

	int type;

	vec3 color;

	bool bEnabled;
	
	vec3 direction;

	float cutOff;

	float ambiant;
	float diffuse;
	float specular;
};

layout (binding = 3) buffer LightBuffer
{
	Light lights[];
} lightBuffer;


layout(binding = 4) uniform sampler2D texSamplers[3];


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


	// Opaque.
	if(alphaModel == 0)
	{
		outColor.a = 1.0;
		return;
	}


	// Allows transparency.
	outColor.a = texture(texSamplers[0], fsIn.texture).a * matConsts.alpha;

	if(outColor.a < 0.001)
		discard;
}


struct IlluminationData
{
	// Albedo from texture.
	vec3 albedo;

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

vec3 ComputeLight(Light _light, IlluminationData _data);
vec3 PBRIllumination(IlluminationData _data);
vec3 BlinnPhongIllumination(IlluminationData _data);

float Fresnel(float _shininess, float _cosTheta);

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
	outColor.xyz = vec3(0);


	// Albedo from texture.
	data.albedo = texture(texSamplers[0], fsIn.texture).xyz;

	// Normal vector.
	data.vNorm = normalize(fsIn.normal);

	// Object to camera direction.
	data.vCam = normalize(fsIn.position - fsIn.camPosition);
	

	// Compute each lights.
	for(int i = 0; i < lightBuffer.lights.length(); ++i)
	{
		if(lightBuffer.lights[i].bEnabled)
			outColor.xyz += ComputeLight(lightBuffer.lights[i], data);
	}
}

vec3 ComputeLight(Light _light, IlluminationData _data)
{
	// Object to light direction.

	if(_light.type == 0)			// Point light.
		_data.vLight = normalize(_light.position - fsIn.position);
	else if(_light.type == 1)		// Directionnal light.
		_data.vLight = normalize(-_light.direction);
	else if(_light.type == 2)		// Spot light.
		;


	// Halfway vector.
	_data.vHalf = normalize(_data.vLight + _data.vCam);


	_data.cosTheta = dot(_data.vNorm, _data.vLight);

	// Blinn-Phong variant. Phong formula is: dot(vNorm, vCam)
	_data.cosAlpha = max(dot(_data.vNorm, _data.vHalf), 0.0);


	// Ambiant component.
	_data.Ra = _light.color * _light.ambiant * matConsts.ka;
	
	// Diffuse component.
	_data.Rd = _light.color * _light.diffuse * matConsts.kd * mix(_data.albedo, vec3(0.0), matConsts.metallic);

	// Specular component.
	_data.Rs = _light.color * _light.specular * matConsts.ks * mix(vec3(1.0), _data.albedo, matConsts.metallic);


	// Illumination model.
	if(illumModel == 1)
		return BlinnPhongIllumination(_data);	// Simple Blinn-Phong implementation.
	else if(illumModel == 2)
		return PBRIllumination(_data);			// Physically based using BRDF.
}

vec3 BlinnPhongIllumination(IlluminationData _data)
{
	// Attenuation constants.
	const float kc = 0.01;	// Constant.
	const float kl = 0.2;	// Linear.
	const float kq = 0.2;	// Quadratic.
	

	// Normal facing light.
	float facing = _data.cosTheta > 0.0 ? 1.0 : 0.0;

	vec3 diffuse = _data.Rd * max(_data.cosTheta, 0.0);
	vec3 specular = _data.Rs * pow(_data.cosAlpha, matConsts.shininess) * facing;  
	
	// TODO: Attenuation for Directionnal and Spot light?
	float attenuation = 1.0 / (kc + kl * length(_data.vLight) + kq * dot(_data.vLight, _data.vLight));
	

	//  Output.
	return _data.Ra + attenuation * (diffuse + specular);
}


vec3 PBRIllumination(IlluminationData _data)
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
	vec3 specularBRDF = _data.Rs * normFactor * Fresnel(matConsts.shininess, dot(_data.vCam, _data.vHalf)) * G * pow(_data.cosAlpha, matConsts.shininess);
	

	//  Output.
	return _data.Ra + (diffuseBRDF + specularBRDF) * _data.cosTheta;
}


float Fresnel(float _shininess, float _cosTheta)
{
	// f0: Material reflectance at 0 degree.
	const float f0 = pow((1 - _shininess) / (1 + _shininess), 2);


	// Schlick's approximation.
	return f0 + (1 - f0) * pow(1 - _cosTheta, 5);
}
