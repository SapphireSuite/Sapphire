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

		std::vector<ImageBuffer> mAttachments;
		std::vector<ImageBuffer> mInputAttachments;
		std::vector<VkClearValue> mClearValues;

		void AddClearColor(Format _format, const Color& _clearColor);

	public:
		CommandBuffer commandBuffer;

		const IImageBuffer& GetInputAttachment(uint32 _index) const override final;

		void Create(const IRenderInstance& _instance, const FrameBufferCreateInfos& _infos) override final;
		void Create(const IRenderInstance& _instance, const FrameBufferCreateInfos& _infos, VkImage presentImage);
		void Destroy(const IRenderInstance& _instance) override final;

		void Begin() override final;
		void NextSubpass() override final;
		void End() override final;
	};
}

#endif

#endif // GUARD
