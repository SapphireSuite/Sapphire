// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Framebuffer.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Core/Algorithms/SizeOf.hpp>

namespace Sa::vk
{
	Framebuffer::Framebuffer(const RenderPass* _renderPass, const ImageExtent& _extent)
		: mRenderPass{ _renderPass }, mExtent { _extent }
	{
		VkDevice device = VkRenderInstance::GetInstance()->AsPtr<VkRenderInstance>()->GetDevice();

		// hardcoded values for now
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, _renderPass->GetColorFormat(), _renderPass->GetSampleBits()));
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, _renderPass->GetColorFormat()));
		mBuffers.emplace_back(VkImageBuffer::CreateDepthBuffer(device, _extent));

		mClearValues.emplace_back(VkClearValue{ 0.2f, 0.2f, 1.f, 1.f });
		mClearValues.emplace_back(VkClearValue{ 0.2f, 0.2f, 1.f, 1.f });
		mClearValues.emplace_back(VkClearValue{ 1.f, 0.f });

		std::vector<VkImageView> attachements(3);
		attachements.emplace_back(mBuffers[0]);
		attachements.emplace_back(mBuffers[1]);
		attachements.emplace_back(mBuffers[2]);

		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = _renderPass->Get();
		framebufferCreateInfo.attachmentCount = SizeOf(attachements);
		framebufferCreateInfo.pAttachments = attachements.data();
		framebufferCreateInfo.width = mExtent.width;
		framebufferCreateInfo.height = mExtent.height;
		framebufferCreateInfo.layers = 1;

		SA_VK_ASSERT(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &mFramebuffer),
							CreationFailed, Rendering, L"Failed to create framebuffer!");

		VkCommandBufferAllocateInfo commandBufferAllocInfo{};
		commandBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocInfo.pNext = nullptr;
		commandBufferAllocInfo.commandPool = device.GetGraphicsQueue().GetCommandPool();
		commandBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocInfo.commandBufferCount = 1;

		SA_VK_ASSERT(vkAllocateCommandBuffers(device, &commandBufferAllocInfo, &mPrimaryCommandBuffer.Get()),
			CreationFailed, Rendering, L"Failed to allocate command buffers!");
	}

	Framebuffer::~Framebuffer()
	{
		VkDevice device = VkRenderInstance::GetInstance()->AsPtr<VkRenderInstance>()->GetDevice();

		vkFreeCommandBuffers(device, device.GetGraphicsQueue().GetCommandPool(), 1, &mPrimaryCommandBuffer.Get());

		for (size_t i = 0; i < mBuffers.size(); ++i)
			mBuffers[i].Destroy(device);
	}

	const CommandBuffer& Framebuffer::Begin() const
	{
		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = mRenderPass->Get();
		renderPassBeginInfo.framebuffer = mFramebuffer;
		renderPassBeginInfo.renderArea = VkRect2D{ VkOffset2D{}, mExtent };
		renderPassBeginInfo.clearValueCount = SizeOf(mClearValues);
		renderPassBeginInfo.pClearValues = mClearValues.data();
		
		vkCmdBeginRenderPass(mPrimaryCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		return mPrimaryCommandBuffer;
	}

	void Framebuffer::End() const
	{
		vkCmdEndRenderPass(mPrimaryCommandBuffer);
	}
}
