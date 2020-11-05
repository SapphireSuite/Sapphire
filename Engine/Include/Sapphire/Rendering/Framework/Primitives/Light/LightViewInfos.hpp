// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_LIGHT_VIEW_INFOS_GUARD
#define SAPPHIRE_RENDERING_LIGHT_VIEW_INFOS_GUARD

#include <Maths/Space/Matrix4.hpp>

namespace Sa
{
	struct LightViewInfos
	{
		Mat4f proj;

		Vec3f position;

		float farPlane = 1.0f;
	};
}

#endif // GUARD
