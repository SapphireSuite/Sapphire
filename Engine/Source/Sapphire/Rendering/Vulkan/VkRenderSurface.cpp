// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/VkRenderSurface.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	VkRenderSurface::operator VkSurfaceKHR&()
	{
		return mHandle;
	}

	VkRenderSurface::operator VkSurfaceKHR() const
	{
		return mHandle;
	}
}

#endif
