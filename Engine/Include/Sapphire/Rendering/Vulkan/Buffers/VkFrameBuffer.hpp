// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_FRAME_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_FRAME_BUFFER_GUARD

#include <Rendering/Framework/System/RenderPass/RenderPassDescriptor.hpp>

#include <Rendering/Vulkan/Buffers/VkImageBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;
	class RenderPass;
	//class SwapChain;

	class FrameBuffer
	{
		VkFramebuffer mHandle = VK_NULL_HANDLE;

		std::vector<ImageBuffer> mBuffers;
		std::vector<VkClearValue> mClearValues;

	public:
		void Create(const Device& _device, const RenderPass& _renderPass,
			const RenderPassDescriptor& _rpDescriptor,
			const Vec2ui& _extent, VkImage presentImage = VK_NULL_HANDLE);
		void Destroy(const Device& _device);
	};
}

#endif

#endif // GUARD
