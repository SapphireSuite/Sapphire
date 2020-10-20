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

	class VkRenderPass : public IRenderPass
	{
		::VkRenderPass mHandle = VK_NULL_HANDLE;

		ImageExtent mExtent;
	//TODO Aurel: make it private and handle any number of buffer
	public:
		VkImageBuffer mDepthBuffer;
		VkImageBuffer mColorMultisamplingBuffer;

	private:
		SampleBits mSampleBits = SampleBits::Sample1Bit;

		std::vector<VkClearValue> mClearValues;

		void SetSampleCount(const VkDevice& _device, SampleBits _desiredSampling);

		void CreateDepthBuffer(const VkDevice& _device);
		void DestroyDepthBuffer(const VkDevice& _device);

		void CreateColorMultisamplingBuffer(const VkDevice& _device, const VkFormat _imageFormat);
		void DestroyColorMultisamplingBuffer(const VkDevice& _device);

	public:
		SampleBits GetSampleBits() const noexcept;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API VkUniformBuffer& GetUniformBuffer(uint32 _index);
		const std::vector<VkUniformBuffer>& GetStaticUniformBuffers() const;

		void SetClearColor(const Color& _color) override final;

		void Create(const IRenderInstance& _instance, const RenderPassCreateInfos& _createInfos) override final;
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
