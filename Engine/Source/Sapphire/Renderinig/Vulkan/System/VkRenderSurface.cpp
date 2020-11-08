// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderSurface.hpp>

namespace Sa::Vk
{
	RenderSurface::operator VkSurfaceKHR() const
	{
		return mHandle;
	}
}
