#version 450
#extension GL_ARB_separate_shader_objects : enable


// Uniform.
layout(binding = 2) uniform sampler2D texSamplers[3];


layout(binding = 3) uniform MaterialConstants
{
	// Ambient constant.
	vec3 ambient;
	
	// Transparency.
	float alpha;

	// Diffuse constant.
    vec3 diffuse;

	float shininess;

	// Specular constant.
	vec3 specular;

	float metallic;

	// Emissive constant.
	vec3 emissive;

	// Optical density (Refractive index).
	float refraction;

	// Fresnel reflectance.
	float reflectance;
} matConsts;


// Directionnal Light.
struct DirectionnalLight
{
	vec3 direction;

	bool bEnabled;

	vec3 color;
	
	float intensity;

	float ambient;
	float diffuse;
	float specular;
};

layout (binding = 4) buffer DirectionnalLightBuffer
{
	DirectionnalLight lights[];
} dLightBuffer;


// Point Light.
struct PointLight
{
	vec3 position;

	bool bEnabled;

	vec3 color;
	
	float intensity;

	float range;

	float ambient;
	float diffuse;
	float specular;
};

layout (binding = 5) buffer PointLightBuffer
{
	PointLight lights[];
} pLightBuffer;


// Spot Light.
struct SpotLight
{
	vec3 position;

	bool bEnabled;

	vec3 direction;

	float cutOff;

	vec3 color;
	
	float intensity;

	float range;

	float ambient;
	float diffuse;
	float specular;
};

layout (binding = 6) buffer SpotLightBuffer
{
	SpotLight lights[];
} sLightBuffer;


// Constants.
const float PI = 3.14159265359;

layout(constant_id = 0) const int alphaModel = 0;
layout(constant_id = 1) const int illumModel = 0;


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


	// Ambient component.
	vec3 Ra;
	
	// Diffuse component.
	vec3 Rd;

	// Specular component.
	vec3 Rs;


	// Fresnel f0 (reflectance at 0).
	vec3 f0;
};

void ComputeLights(IlluminationData _data);
vec3 ComputeIlluminationModel(IlluminationData _data);

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
	if(texture(texSamplers[1], fsIn.texture).rgb != vec3(0))
	{
		vec3 N = normalize(fsIn.normal);
		vec3 T = normalize(fsIn.tangent);
		vec3 B = /*handness */ -1.0 * cross (N, T);
		mat3 TBN = mat3(T, B, N);
		data.vNorm = normalize(TBN * (texture(texSamplers[1], fsIn.texture).rgb * 2.0 - 1.0));
	}
	else
		data.vNorm = normalize(fsIn.normal);


	// Object to camera direction.
	data.vCam = normalize(fsIn.viewPosition - fsIn.position);
	

	ComputeLights(data);
}


void FillIlluminationData(inout IlluminationData _data, vec3 _color, float _ambient, float _diffuse, float _specular)
{
	// Halfway vector.
	_data.vHalf = normalize(_data.vLight + _data.vCam);


	_data.cosTheta = dot(_data.vNorm, _data.vLight);

	// Blinn-Phong variant. Phong formula is: dot(vNorm, vCam)
	_data.cosAlpha = max(dot(_data.vNorm, _data.vHalf), 0.0);


	// Ambient component.
	_data.Ra = _color * _ambient * matConsts.ambient * _data.albedo;
	
	// Diffuse component.
	_data.Rd = _color * _diffuse * matConsts.diffuse * mix(_data.albedo, vec3(0.0), matConsts.metallic);

	// Specular component.
	_data.Rs = _color * _specular * matConsts.specular * mix(vec3(1.0), _data.albedo, matConsts.metallic);


	// Fresnel reflectance.
	_data.f0 = mix(vec3(0.16 * matConsts.reflectance * matConsts.reflectance), _data.albedo, matConsts.metallic);
}


// Blinn-Phong illumination.
vec3 BlinnPhongIllumination(IlluminationData _data)
{
	// Normal facing light.
	float facing = _data.cosTheta > 0.0 ? 1.0 : 0.0;

	vec3 diffuse = _data.Rd * max(_data.cosTheta, 0.0);
	vec3 specular = _data.Rs * pow(_data.cosAlpha, matConsts.shininess) * facing;
	
	//  Output.
	return _data.Ra + diffuse + specular;
}


// PBR illumination.
vec3 Fresnel(vec3 _f0, float _cosTheta)
{
	// Schlick's approximation.
	return _f0 + (1 - _f0) * pow(1 - _cosTheta, 5);
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
	vec3 specularBRDF = _data.Rs * normFactor * Fresnel(_data.f0, dot(_data.vCam, _data.vHalf)) * G * pow(_data.cosAlpha, matConsts.shininess);
	

	//  Output.
	return max(_data.Ra + (diffuseBRDF + specularBRDF) * _data.cosTheta, vec3(0.0));
}

float ComputeAttenuation(vec3 _lightPosition, float _lightRange)
{
	float distance = length(_lightPosition - fsIn.position);

	return max(1 - (distance / _lightRange), 0.0);
}

vec3 ComputeDirectionnalLight(DirectionnalLight _light, IlluminationData _data)
{
	// Object to light direction.
	_data.vLight = normalize(-_light.direction);

	FillIlluminationData(_data, _light.color * _light.intensity, _light.ambient, _light.diffuse, _light.specular);

	return ComputeIlluminationModel(_data);
}

vec3 ComputePointLight(PointLight _light, IlluminationData _data)
{
	// Object to light direction.
	_data.vLight = normalize(_light.position - fsIn.position);

	FillIlluminationData(_data, _light.color * _light.intensity, _light.ambient, _light.diffuse, _light.specular);

	return ComputeAttenuation(_light.position, _light.range) * ComputeIlluminationModel(_data);
}

vec3 ComputeSpotLight(SpotLight _light, IlluminationData _data)
{
	// Object to light direction.
	_data.vLight = normalize(_light.position - fsIn.position);


	float theta = dot(_data.vLight, normalize(_light.direction));

	// Fragment not in spot light.
	if(theta < _light.cutOff)
		return _data.Ra;


	FillIlluminationData(_data, _light.color* _light.intensity, _light.ambient, _light.diffuse, _light.specular);


	return ComputeAttenuation(_light.position, _light.range) * ComputeIlluminationModel(_data);
}


vec3 ComputeIlluminationModel(IlluminationData _data)
{
	// Illumination model.

	if(illumModel == 1)
		return BlinnPhongIllumination(_data);					// Simple Blinn-Phong implementation.
	else if(illumModel == 2)
		return PBRIllumination(_data);							// Physically based using BRDF.
}

void ComputeLights(IlluminationData _data)
{
	// Directionnal Lights.
	for(int i = 0; i < dLightBuffer.lights.length(); ++i)
	{
		if(dLightBuffer.lights[i].bEnabled)
			outColor.xyz += ComputeDirectionnalLight(dLightBuffer.lights[i], _data);
	}

	// Point Lights.
	for(int i = 0; i < pLightBuffer.lights.length(); ++i)
	{
		if(pLightBuffer.lights[i].bEnabled)
			outColor.xyz += ComputePointLight(pLightBuffer.lights[i], _data);
	}


	// Spot Lights.
	for(int i = 0; i < sLightBuffer.lights.length(); ++i)
	{
		if(sLightBuffer.lights[i].bEnabled)
			outColor.xyz += ComputeSpotLight(sLightBuffer.lights[i], _data);
	}
}