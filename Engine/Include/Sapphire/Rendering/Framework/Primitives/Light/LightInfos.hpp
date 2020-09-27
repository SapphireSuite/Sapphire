// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_LIGHT_INFOS_GUARD
#define SAPPHIRE_RENDERING_LIGHT_INFOS_GUARD

#include <Core/Support/Pragma.hpp>

#include <Maths/Space/Vector3.hpp>

SA_PRAGMA_SDWARN("", 4324)

namespace Sa
{
	enum class LightType
	{
		Point,

		Directionnal,

		Spot
	};

	struct LightInfos
	{
		// Used for Point and Spot.
		Vec3f position;
		
		LightType type;

		// Lighting color.
		alignas(16) Vec3f color = Vec3f::One;

		bool bEnable = true;

		// Used for Directionnal and Spot.
		alignas(16) Vec3f direction = Vec3f::Forward;

		// Used for Spot (use empty align space).
		float cutOff = 60.0f;

		// Lighting components.
		float ambiant = 0.01f;
		float diffuse = 0.64f;
		float specular = 0.35f;
	};
}

SA_PRAGMA_EDWARN()

#endif // GUARD
