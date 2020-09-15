// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Framework/Primitives/IRenderSurface.hpp>

#include <Rendering/Vulkan/Primitives/VkSwapChain.hpp>
#include <Rendering/Vulkan/Primitives/VkRenderPass.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	class VkRenderSurface : public IRenderSurface
	{
		VkSurfaceKHR mHandle = VK_NULL_HANDLE;

		VkSwapChain mSwapChain;
		VkRenderPass mRenderPass;

	public:
		void InitHandle(VkSurfaceKHR _newHandle);
		void UnInitHandle();

		const VkSwapChain& GetSwapChain() const;
		const VkRenderPass& GetRenderPass() const;

		void Create(const VkDevice& _device, const VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy(const VkDevice& _device);

		const ImageExtent& GetImageExtent() const override final;

		operator VkSurfaceKHR() const;
	};
}

#endif

#endif // GUARD
