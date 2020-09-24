// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_MATERIAL_CONSTANTS_GUARD
#define SAPPHIRE_RENDERING_MATERIAL_CONSTANTS_GUARD

#include <Core/Support/Pragma.hpp>

#include <Maths/Space/Vector3.hpp>

SA_PRAGMA_SDWARN("", 4324)


namespace Sa
{
	struct MaterialConstants
	{
		// Ambiant constant.
		alignas(16) Vec3f ka = Vec3f::One;

		// Diffuse constant.
		alignas(16) Vec3f kd = Vec3f::One;

		// Specular constant.
		alignas(16) Vec3f ks = Vec3f::One;

		// Emissive constant.
		alignas(16) Vec3f ke;

		// Transmission filter.
		alignas(16) Vec3f tf = Vec3f::One;


		// Used for shininess.
		float roughness = 0.0f;

		// Transparency.
		float alpha = 1.0;

		// Optical density (Refractive index).
		float refractIndex = 1.0f;


		// Set roughness from shininess.
		void SetShininess(float _shininess);

		// Set roughness from smoothness.
		void SetSmoothness(float _smoothness);
	};
}

SA_PRAGMA_EDWARN()

#endif // GUARD
