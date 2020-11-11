// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	void FrameBuffer::Create(const Device& _device, const RenderPass& _renderPass,
		const RenderPassDescriptor& _rpDescriptor,
		const Vec2ui& _extent, uint32 _poolIndex, VkImage presentImage)
	{
#if SA_DEBUG
		if (_rpDescriptor.bPresent)
			SA_ASSERT(presentImage != VK_NULL_HANDLE, InvalidParam, Rendering, L"Framebuffer with bPresent == true requiere a valid swapchain image!");
#endif

		// === Attachment buffers ===
		ImageBufferCreateInfos imageInfos;
		imageInfos.extent = _extent;
		imageInfos.sampling = _rpDescriptor.sampling;

		// Add Image buffer for each attachment.
		imageInfos.usage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

		auto subEndIt = _rpDescriptor.subPassDescriptors.end();
		
		// Prensent subpass framebuffer should be created manually.
		if (_rpDescriptor.bPresent)
			--subEndIt;

		for (auto subIt = _rpDescriptor.subPassDescriptors.begin(); subIt != subEndIt; ++subIt)
		{
			for (auto attIt = subIt->attachmentDescriptors.begin(); attIt != subIt->attachmentDescriptors.end(); ++attIt)
			{
				imageInfos.format = attIt->format;

				mBuffers.emplace_back(ImageBuffer{}).Create(_device, imageInfos);
			}
		}

		// === Present buffer ===
		if (_rpDescriptor.bPresent)
		{
			auto& attachments = subEndIt->attachmentDescriptors;

			imageInfos.format = attachments[0].format;
			mBuffers.emplace_back(ImageBuffer{}).Create(_device, imageInfos);

			// Multisampling resolution bufffer.
			if (_rpDescriptor.sampling != SampleBits::Sample1Bit)
			{
				imageInfos.sampling = SampleBits::Sample1Bit;
				mBuffers.emplace_back(ImageBuffer{}).CreateFromImage(_device, imageInfos, presentImage);

				imageInfos.sampling = _rpDescriptor.sampling;
			}
		}

		// Color clear values.
		if (_rpDescriptor.bClear)
			mClearValues.insert(mClearValues.end(), SizeOf(mBuffers), _rpDescriptor.clearColor);


		// === Depth buffer ===
		if (_rpDescriptor.bDepthBuffer)
		{
			imageInfos.usage = 0u;
			imageInfos.format = _rpDescriptor.depthFormat;

			mBuffers.emplace_back(ImageBuffer{}).Create(_device, imageInfos);

			if (_rpDescriptor.bClear)
				mClearValues.emplace_back(VkClearValue{ { { 1.f, 0u } } });
		}


		// === Create FrameBuffer ===
		std::vector<VkImageView> attachements(mBuffers.size());

		for (size_t i = 0; i < mBuffers.size(); ++i)
			attachements[i] = mBuffers[i];

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
		commandBuffer = CommandBuffer::Allocate(_device, QueueFamilyType::Graphics, _poolIndex);
	}

	void FrameBuffer::Destroy(const Device& _device)
	{
		CommandBuffer::Free(_device, commandBuffer);

		vkDestroyFramebuffer(_device, mHandle, nullptr);

		for (auto it = mBuffers.begin(); it != mBuffers.end(); ++it)
			it->Destroy(_device);

		mBuffers.clear();
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