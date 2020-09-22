// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MATERIAL_CREATE_INFOS_GUARD
#define SAPPHIRE_SDK_MATERIAL_CREATE_INFOS_GUARD

#include <Maths/Space/Vector3.hpp>

namespace Sa
{
	struct MaterialCreateInfos
	{
		Vec3f kd;
		Vec3f ka;
		Vec3f ks;
		Vec3f ke;
		Vec3f tf;

		float d = 0.0f;
		float ni = 0.0f;
		float ns = 0.0f;
	};
}

#endif // GUARD
