// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_FRAMEBUFFER_GUARD
#define SAPPHIRE_RENDERING_FRAMEBUFFER_GUARD

#include <vulkan/vulkan.h>

#include <Rendering/Vulkan/Buffer/VkImageBuffer.hpp>
#include <Rendering/Image/ImageExtent.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#include <vector>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::vk
{
	class DummyRenderPass; // TODO  Aurel: use real RenderPass

	class Framebuffer
	{
		std::vector<VkImageBuffer> mBuffers;
		const DummyRenderPass* mRenderPass;
		CommandBuffer mPrimaryCommandBuffer;

		const ImageExtent mExtent; // const for the moment, might change whith the resize's rework
		std::vector<VkClearValue> mClearValues;

	public:
		// We can't create a framebuffer without a renderPass
		Framebuffer() = delete;

		// Should analyse the render pass and create the according buffers
		Framebuffer(const DummyRenderPass* _renderPass, const ImageExtent& _extent);
	
		// Should clean his data
		~Framebuffer();

	public:
		void Begin() const;
		void End() const;
	};
}

#endif

#endif