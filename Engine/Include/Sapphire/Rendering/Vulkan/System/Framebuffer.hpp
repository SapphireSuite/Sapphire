// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_FRAMEBUFFER_GUARD
#define SAPPHIRE_RENDERING_FRAMEBUFFER_GUARD

#include <Rendering/Vulkan/Buffer/VkImageBuffer.hpp>
#include <Rendering/Image/ImageExtent.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#include <vector>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class RenderPass;

	namespace vk
	{
		class Framebuffer
		{
			std::vector<VkImageBuffer> mBuffers;
			const RenderPass* mRenderPass;
			CommandBuffer mPrimaryCommandBuffer;

			VkFramebuffer mFramebuffer = VK_NULL_HANDLE;

			const ImageExtent mExtent; // const for the moment, might change whith the resize's rework
			std::vector<VkClearValue> mClearValues;

		public:
			// We can't create a framebuffer without a renderPass
			Framebuffer() = delete;

			// Should analyse the render pass and create the according buffers
			Framebuffer(const RenderPass* _renderPass, const ImageExtent& _extent);
	
			// Should clean his data
			~Framebuffer();

		public:
			// return command buffer to record
			const CommandBuffer& Begin() const;
			void End() const;
		};
	}
}

#endif

#endif