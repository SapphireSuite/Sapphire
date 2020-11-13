// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	const ImageBuffer& FrameBuffer::GetAttachment(uint32 _index) const
	{
		SA_ASSERT(_index < SizeOf(mAttachments), OutOfRange, Rendering, _index, 0u, SizeOf(mAttachments));

		return mAttachments[_index];
	}

	void FrameBuffer::Create(const Device& _device, const RenderPass& _renderPass,
		const RenderPassDescriptor& _rpDescriptor,
		const Vec2ui& _extent, uint32 _poolIndex, VkImage presentImage)
	{
#if SA_DEBUG
		if (_rpDescriptor.bPresent)
			SA_ASSERT(presentImage != VK_NULL_HANDLE, InvalidParam, Rendering, L"Framebuffer with bPresent == true requiere a valid swapchain image!");
#endif

		// === Color attachments ===
		ImageBufferCreateInfos imageInfos;
		imageInfos.extent = _extent;
		imageInfos.sampling = _rpDescriptor.sampling;

		// Add Image buffer for each attachment.
		imageInfos.usage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

		auto subEndIt = _rpDescriptor.subPassDescs.end();
		
		// Prensent subpass framebuffer should be created manually.
		if (_rpDescriptor.bPresent)
			--subEndIt;

		for (auto subIt = _rpDescriptor.subPassDescs.begin(); subIt != subEndIt; ++subIt)
		{
			for (auto attIt = subIt->attachmentDescs.begin(); attIt != subIt->attachmentDescs.end(); ++attIt)
			{
				imageInfos.format = attIt->format;

				mAttachments.emplace_back(ImageBuffer{}).Create(_device, imageInfos);
			}
		}

		// === Present attachment ===
		if (_rpDescriptor.bPresent)
		{
			auto& attachments = subEndIt->attachmentDescs;

			imageInfos.format = attachments[0].format;

			if (_rpDescriptor.sampling == SampleBits::Sample1Bit)
				mAttachments.emplace_back(ImageBuffer{}).CreateFromImage(_device, imageInfos, presentImage);
			else
			{
				mAttachments.emplace_back(ImageBuffer{}).Create(_device, imageInfos);

				// Multisampling resolution bufffer.
				imageInfos.sampling = SampleBits::Sample1Bit;
				mAttachments.emplace_back(ImageBuffer{}).CreateFromImage(_device, imageInfos, presentImage);

				imageInfos.sampling = _rpDescriptor.sampling;
			}
		}

		// Color clear values.
		if (_rpDescriptor.bClear)
			mClearValues.insert(mClearValues.end(), SizeOf(mAttachments), _rpDescriptor.clearColor);


		// === Depth attachment ===
		if (_rpDescriptor.bDepthBuffer)
		{
			imageInfos.usage = 0u;
			imageInfos.format = _rpDescriptor.depthFormat;

			mAttachments.emplace_back(ImageBuffer{}).Create(_device, imageInfos);

			if (_rpDescriptor.bClear)
				mClearValues.emplace_back(VkClearValue{ { { 1.f, 0u } } });
		}


		// === Create FrameBuffer ===
		std::vector<VkImageView> attachements(mAttachments.size());

		for (size_t i = 0; i < mAttachments.size(); ++i)
			attachements[i] = mAttachments[i];

		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = _renderPass;
		framebufferCreateInfo.attachmentCount = SizeOf(attachements);
		framebufferCreateInfo.pAttachments = attachements.data();
		framebufferCreateInfo.width = _extent.x;
		framebufferCreateInfo.height = _extent.y;
		framebufferCreateInfo.layers = 1;

		SA_VK_ASSERT(vkCreateFramebuffer(_device, &framebufferCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create framebuffer!");

		mExtent = _extent;
		mRenderPass = _renderPass;

		// === Create command buffer ===
		commandBuffer = CommandBuffer::Allocate(_device, QueueType::Graphics, _poolIndex);
	}

	void FrameBuffer::Destroy(const Device& _device)
	{
		CommandBuffer::Free(_device, commandBuffer);

		vkDestroyFramebuffer(_device, mHandle, nullptr);

		for (auto it = mAttachments.begin(); it != mAttachments.end(); ++it)
			it->Destroy(_device);

		mAttachments.clear();
	}

	void FrameBuffer::Begin()
	{
		vkResetCommandBuffer(commandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

		// === Start Command buffer record ===
		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.pNext = nullptr;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		commandBufferBeginInfo.pInheritanceInfo = nullptr;

		SA_VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo),
			LibCommandFailed, Rendering, L"Failed to begin command buffer!");


		// === Start RenderPass record ===
		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = mRenderPass;
		renderPassBeginInfo.framebuffer = mHandle;
		renderPassBeginInfo.renderArea = VkRect2D{ VkOffset2D{}, VkExtent2D{ mExtent.x, mExtent.y } };
		renderPassBeginInfo.clearValueCount = SizeOf(mClearValues);
		renderPassBeginInfo.pClearValues = mClearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void FrameBuffer::NextSubpass()
	{
		vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
	}

	void FrameBuffer::End()
	{
		// === End RenderPass record ===
		vkCmdEndRenderPass(commandBuffer);


		// === End Command buffer record ===
		SA_VK_ASSERT(vkEndCommandBuffer(commandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");
	}
}

#endif