// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_CONSTANTS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_CONSTANTS_GUARD

#include <Maths/Space/Vector3.hpp>

namespace Sa
{
	struct MaterialConstants
	{
		// Ambiant constant.
		alignas(16) Vec3f ka;

		// Diffuse constant.
		alignas(16) Vec3f kd;

		// Specular constant.
		alignas(16) Vec3f ks;

		alignas(16) Vec3f ke;

		alignas(16) Vec3f tf;

		float d = 0.0f;
		float ni = 0.0f;
		float ns = 0.0f;
	};
}

#endif // GUARD
