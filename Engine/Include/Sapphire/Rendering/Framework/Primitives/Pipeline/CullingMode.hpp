// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_CULLING_MODE_GUARD
#define SAPPHIRE_RENDERING_CULLING_MODE_GUARD

#include <Rendering/APIConfig.hpp>

namespace Sa
{
	enum class CullingMode
	{
		// No culling.
		None,

		// Front-face culling.
		Front,

		// Back-face culling.
		Back,
	};


#if SA_RENDERING_API == SA_VULKAN

	VkCullModeFlags API_GetCullingMode(CullingMode _mode) noexcept;

#endif

}

#endif // GUARD
