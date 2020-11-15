// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	const ImageBuffer& FrameBuffer::GetInputAttachment(uint32 _index) const
	{
		SA_ASSERT(_index < SizeOf(mInputAttachments), OutOfRange, Rendering, _index, 0u, SizeOf(mInputAttachments));

		return mInputAttachments[_index];
	}

	void FrameBuffer::Create(const Device& _device, const RenderPass& _renderPass,
		const RenderPassDescriptor& _rpDescriptor,
		const Vec2ui& _extent, uint32 _poolIndex, VkImage presentImage)
	{
		std::vector<VkImageView> attachementCreateInfos;

		ImageBufferCreateInfos imageInfos;
		imageInfos.extent = _extent;

		for (auto subIt = _rpDescriptor.subPassDescs.begin(); subIt != _rpDescriptor.subPassDescs.end(); ++subIt)
		{
			for (auto attIt = subIt->attachmentDescs.begin(); attIt != subIt->attachmentDescs.end(); ++attIt)
			{
				imageInfos.format = attIt->format;
				imageInfos.usage = 0u; // Reset usage.
				imageInfos.sampling = subIt->sampling; // Reset sampling value.


				if (subIt->sampling != SampleBits::Sample1Bit && !IsDepthFormat(attIt->format))
				{
					// Add multisampled buffer.
					ImageBuffer& multSamplBuffer = mAttachments.emplace_back(ImageBuffer{});
					multSamplBuffer.Create(_device, imageInfos);
					attachementCreateInfos.push_back(multSamplBuffer);

					if (_rpDescriptor.bClear)
						AddClearColor(attIt->format, _rpDescriptor.clearColor);

					imageInfos.sampling = SampleBits::Sample1Bit;
				}


				if (IsPresentFormat(attIt->format))
				{
					SA_ASSERT(presentImage != VK_NULL_HANDLE, InvalidParam, Rendering, L"Framebuffer with present format requiere a valid swapchain image!");
					
					ImageBuffer& presentBuffer = mAttachments.emplace_back(ImageBuffer{});
					presentBuffer.CreateFromImage(_device, imageInfos, presentImage);
					attachementCreateInfos.push_back(presentBuffer);
				}
				else if (attIt->bInputNext)
				{
					imageInfos.usage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

					ImageBuffer& inputBuffer = mInputAttachments.emplace_back(ImageBuffer{});
					inputBuffer.Create(_device, imageInfos);
					attachementCreateInfos.push_back(inputBuffer);
				}
				else
				{
					ImageBuffer& buffer = mAttachments.emplace_back(ImageBuffer{});
					buffer.Create(_device, imageInfos);
					attachementCreateInfos.push_back(buffer);
				}


				if (_rpDescriptor.bClear)
					AddClearColor(attIt->format, _rpDescriptor.clearColor);
			}
		}


		// === Create FrameBuffer ===
		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = _renderPass;
		framebufferCreateInfo.attachmentCount = SizeOf(attachementCreateInfos);
		framebufferCreateInfo.pAttachments = attachementCreateInfos.data();
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

		// Destroy attachments.
		for (auto it = mAttachments.begin(); it != mAttachments.end(); ++it)
			it->Destroy(_device);

		mAttachments.clear();

		// Destroy input attachments.
		for (auto it = mInputAttachments.begin(); it != mInputAttachments.end(); ++it)
			it->Destroy(_device);

		mInputAttachments.clear();
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


	void FrameBuffer::AddClearColor(Format _format, const Color& _clearColor)
	{
		if (IsDepthFormat(_format))
			mClearValues.emplace_back(VkClearValue{ { { 1.f, 0u } } });
		else
			mClearValues.emplace_back(_clearColor);
	}
}

#endif