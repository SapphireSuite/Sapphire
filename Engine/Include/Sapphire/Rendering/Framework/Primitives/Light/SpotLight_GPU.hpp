// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SPOT_LIGHT_GPU_GUARD
#define SAPPHIRE_RENDERING_SPOT_LIGHT_GPU_GUARD

#include <Core/Support/Pragma.hpp>

#include <Maths/Space/Vector3.hpp>

SA_PRAGMA_SDWARN("", 4324) // Remove alignas warning.

namespace Sa
{
	struct SpotLight_GPU
	{
		Vec3f position;

		bool bEnabled = true;

		alignas(16) Vec3f direction = Vec3f::Forward;

		float cutOff = 60.0f;

		// Lighting color.
		alignas(16) Vec3f color = Vec3f::One;


		float intensity = 1.0f;

		// Max range.
		float range = 10.0f;


		// Lighting components.
		float ambient = 0.01f;

		float diffuse = 0.64f;

		float specular = 0.35f;
	};
}

SA_PRAGMA_EDWARN()

#endif // GUARD
