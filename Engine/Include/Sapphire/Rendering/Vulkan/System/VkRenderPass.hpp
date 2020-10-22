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
#include <vector>

namespace Sa
{
	class IRenderSurface;

	class RenderPass : public IRenderPass
	{
		VkRenderPass	mRenderPass		= VK_NULL_HANDLE;
		VkFormat		mColorFormat	= VK_FORMAT_UNDEFINED;
		SampleBits		mSampleBits		= SampleBits::Sample1Bit;

	public:
		VkRenderPass	Get() const noexcept;
		VkFormat		GetColorFormat() const noexcept;
		SampleBits		GetSampleBits() const noexcept;

		// TODO Aurel: check to use constructor & destructor instead
		void Create(const IRenderInstance& _instance, const RenderPassCreateInfos& _createInfos) override final;
		void Destroy(const IRenderInstance& _instance) override final;
	};
}

#endif

#endif // GUARD
