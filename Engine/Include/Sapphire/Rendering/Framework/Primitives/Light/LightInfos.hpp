// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_LIGHT_INFOS_GUARD
#define SAPPHIRE_RENDERING_LIGHT_INFOS_GUARD

#include <Maths/Space/Vector3.hpp>

namespace Sa
{
	struct LightInfos
	{
		alignas(16) Vec3f position;

		alignas(16) Vec3f color = Vec3f::One;

		float ambiant = 0.01f;
		float diffuse = 0.64f;
		float specular = 0.35f;

		float shininess = 100.0f;
	};

	//struct DLightInfos : public LightInfos
	//{
	//};

	//struct PLightInfos : public LightInfos
	//{
	//	//Vec3f position;
	//};
}

#endif // GUARD
