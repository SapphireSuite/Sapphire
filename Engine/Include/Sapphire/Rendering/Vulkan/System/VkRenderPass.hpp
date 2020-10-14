// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_PASS_GUARD

#include <Rendering/Config.hpp>
#include <Rendering/Framework/System/RenderPass/IRenderPass.hpp>

#include <Rendering/Vulkan/Buffer/VkImageBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkUniformBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class IRenderSurface;

	class VkSwapChain;

	class VkRenderPass : public IRenderPass
	{
		::VkRenderPass mHandle = VK_NULL_HANDLE;

		ImageExtent mExtent;

		VkImageBuffer mDepthBuffer;
		std::vector<VkFramebuffer> mFrameBuffers;

		void CreateDepthBuffer(const VkDevice& _device, const VkSwapChain& _swapChain, const RenderPassCreateInfos& _createInfos);
		void DestroyDepthBuffer(const VkDevice& _device);

		void CreateFrameBuffers(const VkDevice& _device, const VkSwapChain& _swapChain);
		void DestroyFrameBuffers(const VkDevice& _device);

	public:
		VkClearValue clearValue[2]{
			VkClearValue{ 0.0f, 0.0f, 0.07f, 1.0f },	// VkClearColorValue.
			VkClearValue{ 1.0f, 0 }						// VkClearDepthStencilValue.
		};


		// Same as SwapChain image num.
		uint32 GetImageNum() const noexcept;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API VkUniformBuffer& GetUniformBuffer(uint32 _index);
		const std::vector<VkUniformBuffer>& GetStaticUniformBuffers() const;

		void Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const RenderPassCreateInfos& _createInfos) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void Begin(const IRenderFrame& _frame) override final;
		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API void End(const IRenderFrame& _frame) override final;

		//// Optimized re-creation.
		//void ReCreate(const IRenderInstance& _instance, const IRenderSurface& _surface);

		operator ::VkRenderPass() const noexcept;
	};
}

#endif

#endif // GUARD
