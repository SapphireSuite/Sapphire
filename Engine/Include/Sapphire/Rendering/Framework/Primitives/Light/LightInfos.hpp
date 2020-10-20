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
		Directionnal,

		Point,

		Spot,

		Max
	};

	// Point light infos.
	struct PLightInfos
	{
		Vec3f position;

		bool bEnabled = true;

		// Lighting color.
		alignas(16) Vec3f color = Vec3f::One;


		float intensity = 1.0f;

		// Max range.
		float range = 10.0f;


		// Lighting components.
		float ambient = 0.0f;

		float diffuse = 0.65f;

		float specular = 0.35f;
	};


	// Directionnal light infos.
	struct DLightInfos
	{
		// Used for Directionnal and Spot.
		alignas(16) Vec3f direction = Vec3f::Forward;

		bool bEnabled = true;

		// Lighting color.
		alignas(16) Vec3f color = Vec3f::One;

		float intensity = 1.0f;


		// Lighting components.
		float ambient = 0.01f;

		float diffuse = 0.64f;

		float specular = 0.35f;
	};


	// Spot light infos.
	struct SLightInfos
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
