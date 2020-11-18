// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkFrameBuffer.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	const IImageBuffer& FrameBuffer::GetInputAttachment(uint32 _index) const
	{
		SA_ASSERT(_index < SizeOf(mInputAttachments), OutOfRange, Rendering, _index, 0u, SizeOf(mInputAttachments));

		return mInputAttachments[_index];
	}

	void FrameBuffer::Create(const IRenderInstance& _instance, const FrameBufferCreateInfos& _infos)
	{
		Create(_instance, _infos, VK_NULL_HANDLE);
	}

	void FrameBuffer::Create(const IRenderInstance& _instance, const FrameBufferCreateInfos& _infos, VkImage presentImage)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		std::vector<VkImageView> attachementCreateInfos;

		ImageBufferCreateInfos imageInfos;
		imageInfos.extent = _infos.extent;

		for (auto subIt = _infos.rpDescriptor.subPassDescs.begin(); subIt != _infos.rpDescriptor.subPassDescs.end(); ++subIt)
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
					multSamplBuffer.Create(device, imageInfos);
					attachementCreateInfos.push_back(multSamplBuffer);

					if (_infos.rpDescriptor.bClear)
						AddClearColor(attIt->format, _infos.rpDescriptor.clearColor);

					imageInfos.sampling = SampleBits::Sample1Bit;
				}


				if (IsPresentFormat(attIt->format) && presentImage != VK_NULL_HANDLE)
				{
					ImageBuffer& presentBuffer = mAttachments.emplace_back(ImageBuffer{});

					presentBuffer.CreateFromImage(device, imageInfos, presentImage);
					attachementCreateInfos.push_back(presentBuffer);
				}
				else if (attIt->bInputNext)
				{
					imageInfos.usage = VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

					ImageBuffer& inputBuffer = mInputAttachments.emplace_back(ImageBuffer{});
					inputBuffer.Create(device, imageInfos);
					attachementCreateInfos.push_back(inputBuffer);
				}
				else
				{
					ImageBuffer& buffer = mAttachments.emplace_back(ImageBuffer{});
					buffer.Create(device, imageInfos);
					attachementCreateInfos.push_back(buffer);
				}


				if (_infos.rpDescriptor.bClear)
					AddClearColor(attIt->format, _infos.rpDescriptor.clearColor);
			}
		}


		// === Create FrameBuffer ===
		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = _infos.renderPass.As<RenderPass>();
		framebufferCreateInfo.attachmentCount = SizeOf(attachementCreateInfos);
		framebufferCreateInfo.pAttachments = attachementCreateInfos.data();
		framebufferCreateInfo.width = _infos.extent.x;
		framebufferCreateInfo.height = _infos.extent.y;
		framebufferCreateInfo.layers = _infos.layerNum;

		SA_VK_ASSERT(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create framebuffer!");

		mExtent = _infos.extent;
		mRenderPass = _infos.renderPass.As<RenderPass>();

		// === Create command buffer ===
		commandBuffer = CommandBuffer::Allocate(device, QueueType::Graphics, _infos.poolIndex);
	}

	void FrameBuffer::Destroy(const IRenderInstance& _instance)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		CommandBuffer::Free(device, commandBuffer);

		vkDestroyFramebuffer(device, mHandle, nullptr);

		// Destroy attachments.
		for (auto it = mAttachments.begin(); it != mAttachments.end(); ++it)
			it->Destroy(device);

		mAttachments.clear();

		// Destroy input attachments.
		for (auto it = mInputAttachments.begin(); it != mInputAttachments.end(); ++it)
			it->Destroy(device);

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