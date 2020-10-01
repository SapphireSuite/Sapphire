// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Framework/System/IRenderSurface.hpp>

#include <Rendering/Vulkan/System/VkSwapChain.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>

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

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API VkSwapChain& GetSwapChain();
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const VkSwapChain& GetSwapChain() const;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const VkRenderPass& GetRenderPass() const;

		void Create(const VkDevice& _device, const VkQueueFamilyIndices& _queueFamilyIndices);
		void Destroy(const VkDevice& _device);

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API VkFormat GetImageFormat() const noexcept;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API const ImageExtent& GetImageExtent() const noexcept override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API Viewport GetViewport() const noexcept override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height) override final;

		operator VkSurfaceKHR() const;
	};
}

#endif

#endif // GUARD