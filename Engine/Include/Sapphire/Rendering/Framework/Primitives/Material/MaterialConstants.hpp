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

		// Transparency.
		float alpha = 1.0;

		// Diffuse constant.
		alignas(16) Vec3f kd = Vec3f::One;

		float shininess = 10.0f;

		// Specular constant.
		alignas(16) Vec3f ks = Vec3f::One;

		float metallic = 0.35f;

		// Emissive constant.
		alignas(16) Vec3f ke;

		// Optical density (Refractive index).
		float refractIndex = 1.0f;

		// Transmission filter.
		alignas(16) Vec3f tf = Vec3f::One;


		// Set shininess from roughness.
		void SetRoughness(float _roughness);

		// Set shininess from smoothness.
		void SetSmoothness(float _smoothness);
	};
}

SA_PRAGMA_EDWARN()

#endif // GUARD
