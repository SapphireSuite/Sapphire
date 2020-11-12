// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_FRAME_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_FRAME_BUFFER_GUARD

#include <Rendering/Framework/Buffers/IFrameBuffer.hpp>

#include <Rendering/Framework/System/RenderPass/RenderPassDescriptor.hpp>

#include <Rendering/Vulkan/Buffers/VkImageBuffer.hpp>
#include <Rendering/Vulkan/Buffers/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;
	class RenderPass;

	class FrameBuffer : public IFrameBuffer
	{
		VkFramebuffer mHandle = VK_NULL_HANDLE;
		VkRenderPass mRenderPass = VK_NULL_HANDLE;

		Vec2ui mExtent;

		std::vector<ImageBuffer> mBuffers;
		std::vector<VkClearValue> mClearValues;

	public:
		CommandBuffer commandBuffer;

		void Create(const Device& _device, const RenderPass& _renderPass,
			const RenderPassDescriptor& _rpDescriptor,
			const Vec2ui& _extent, uint32 _poolIndex = 0u, VkImage presentImage = VK_NULL_HANDLE);
		void Destroy(const Device& _device);

		void Begin() override final;
		void NextSubpass() override final;
		void End() override final;
	};
}

#endif

#endif // GUARD
