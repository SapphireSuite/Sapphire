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
		Vec3f ni;
		Vec3f ns;

		std::string kdMapName;
	};
}

#endif // GUARD
