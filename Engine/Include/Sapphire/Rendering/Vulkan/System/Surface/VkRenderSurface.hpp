// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_VK_RENDER_SURFACE_GUARD

#include <Rendering/Framework/System/IRenderSurface.hpp>

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Vulkan/System/Surface/VkSwapChain.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class RenderSurface : public IRenderSurface
	{
		VkSurfaceKHR mHandle = VK_NULL_HANDLE;

		SwapChain mSwapChain;

	public:
		struct SupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		RenderSurface(VkSurfaceKHR _handle) noexcept;

		Format GetFormat() const noexcept override final;

		void Create(const IRenderInstance& _instance) override final;
		void Destroy(const IRenderInstance& _instance) override final;

		RenderFrame Begin(const IRenderInstance& _instance) override final;
		void End(const IRenderInstance& _instance) override final;

		const std::vector<IFrameBuffer*>& CreateFrameBuffers(IRenderInstance& _instance, const IRenderPass& _renderPass,
			const RenderPassDescriptor& _renderPassDesc, uint32* _size = nullptr) override final;
		void DestroyFrameBuffers(IRenderInstance& _instance) override final;


		SupportDetails QuerySupportDetails(VkPhysicalDevice _device) const;

		static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const SupportDetails& _details);
		static VkPresentModeKHR ChooseSwapPresentMode(const SupportDetails& _details);
		static Vec2ui ChooseSwapExtent(const SupportDetails& _details);

		operator VkSurfaceKHR() const;
	};
}

#endif

#endif // GUARD
