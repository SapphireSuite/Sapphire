// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_POLYGON_MODE_GUARD
#define SAPPHIRE_RENDERING_POLYGON_MODE_GUARD

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

#endif

namespace Sa
{
	enum class PolygonMode
	{
		// Draw point only.
		Point,

		// Draw line only.
		Line,

		// Draw fill triangle.
		Fill
	};


#if SA_RENDERING_API == SA_VULKAN

	VkPolygonMode API_GetPolygonMode(PolygonMode _mode) noexcept;

#endif

}

#endif // GUARD
