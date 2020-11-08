// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD

#include <Rendering/Framework/System/IRenderSurface.hpp>

#include <Rendering/Vulkan/System/VkSwapChain.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class RenderSurface : public IRenderSurface
	{
		VkSurfaceKHR mHandle = VK_NULL_HANDLE;

		SwapChain mSwapChain;

	public:
		operator VkSurfaceKHR() const;
	};
}

#endif

#endif // GUARD
