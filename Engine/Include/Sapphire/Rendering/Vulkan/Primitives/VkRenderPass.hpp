// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkDevice;
	class VkSwapChain;

	class VkRenderPass
	{
		::VkRenderPass mHandle = VK_NULL_HANDLE;

	public:
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Create(const VkDevice& _device, VkFormat _format);
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Destroy(const VkDevice& _device);

		// Optimized re-creation.
		void ReCreate(const VkDevice& _device, VkFormat _format);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API operator ::VkRenderPass() const noexcept;
	};
}

#endif

#endif // GUARD
