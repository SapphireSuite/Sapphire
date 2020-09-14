// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Framework/RenderSurface.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderSurface : public RenderSurface
	{
		VkSurfaceKHR mHandle = VK_NULL_HANDLE;

	public:
		operator VkSurfaceKHR&();

		operator VkSurfaceKHR() const;
	};
}

#endif

#endif // GUARD
