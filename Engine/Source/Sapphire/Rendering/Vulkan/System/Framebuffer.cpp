// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/Framebuffer.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Core/Algorithms/SizeOf.hpp>

namespace Sa::vk
{
	Framebuffer::Framebuffer(const RenderPass* _renderPass, const ImageExtent& _extent)
		: mExtent { _extent }
	{
		AddRenderPass(_renderPass);

		VkDevice device = VkRenderInstance::GetInstance()->AsPtr<VkRenderInstance>()->GetDevice();

		// hardcoded values for now
		if(_renderPass->GetSampleBits() > SampleBits::Sample1Bit)
			mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, _renderPass->GetColorFormat(),
																	_renderPass->GetSampleBits()));
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, _renderPass->GetColorFormat()));
		mBuffers.emplace_back(VkImageBuffer::CreateDepthBuffer(device, _extent, _renderPass->GetSampleBits()));

		// G-Buffer
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8G8B8A8_UNORM)); // Position
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8G8B8A8_UNORM)); // Normal
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8G8B8_UNORM)); // Albedo
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8_UNORM)); // Metallic
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8_UNORM)); // Roughness
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8_UNORM)); // AO

		Create_Internal();
	}

	Framebuffer::Framebuffer(const RenderPass* _renderPass, const ImageExtent& _extent, VkImageBuffer& _colorBuffer)
		: mExtent{ _extent }
	{
		AddRenderPass(_renderPass);

		VkDevice device = VkRenderInstance::GetInstance()->AsPtr<VkRenderInstance>()->GetDevice();

		// hardcoded values for now
		if (_renderPass->GetSampleBits() > SampleBits::Sample1Bit)
			mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, _renderPass->GetColorFormat(),
																	_renderPass->GetSampleBits()));
		mBuffers.emplace_back(_colorBuffer);
		mBuffers.emplace_back(VkImageBuffer::CreateDepthBuffer(device, _extent, _renderPass->GetSampleBits()));

		// G-Buffer
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8G8B8A8_UNORM)); // Position
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8G8B8A8_UNORM)); // Normal
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8G8B8_UNORM)); // Albedo
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8_UNORM)); // Metallic
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8_UNORM)); // Roughness
		mBuffers.emplace_back(VkImageBuffer::CreateColorBuffer(device, _extent, VK_FORMAT_R8_UNORM)); // AO

		Create_Internal();
	}

	Framebuffer::~Framebuffer()
	{
		VkDevice device = VkRenderInstance::GetInstance()->AsPtr<VkRenderInstance>()->GetDevice();

		vkFreeCommandBuffers(device, device.GetGraphicsQueue().GetCommandPool(), 1, &mPrimaryCommandBuffer.Get());

		for (size_t i = 0; i < mBuffers.size(); ++i)
			mBuffers[i].Destroy(device);
	}

	void Framebuffer::Create_Internal()
	{
		VkDevice device = VkRenderInstance::GetInstance()->AsPtr<VkRenderInstance>()->GetDevice();

		mClearValues.emplace_back(VkClearValue{ 0.1f, 0.1f, 0.7f, 0.f });
		mClearValues.emplace_back(VkClearValue{ 0.1f, 0.1f, 0.7f, 0.f });
		mClearValues.emplace_back(VkClearValue{ 1.f, 0.f });

		std::vector<VkImageView> attachements(mBuffers.size());
		for (size_t i = 0; i < mBuffers.size(); ++i)
			attachements[i] = mBuffers[i];

		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = mRenderPass[0]->Get();
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

	void Framebuffer::AddRenderPass(const RenderPass* _renderPass)
	{
		mRenderPass.emplace_back(_renderPass);
	}

	const CommandBuffer& Framebuffer::GetCommandBuffer() const
	{
		return mPrimaryCommandBuffer;
	}

	void Framebuffer::Begin()
	{
		SA_ASSERT(mRenderPassIndex == 0, OutOfRange, Rendering)

		vkResetCommandBuffer(mPrimaryCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.pNext = nullptr;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		commandBufferBeginInfo.pInheritanceInfo = nullptr;

		SA_VK_ASSERT(vkBeginCommandBuffer(mPrimaryCommandBuffer, &commandBufferBeginInfo),
			LibCommandFailed, Rendering, L"Failed to begin command buffer!");

		VkViewport viewport = {};
		viewport.width = mExtent.width;
		viewport.height = mExtent.height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;
		vkCmdSetViewport(mPrimaryCommandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset = { 0,0 };
		scissor.extent = { mExtent.width, mExtent.height };
		vkCmdSetScissor(mPrimaryCommandBuffer, 0, 1, &scissor);

		Next();
	}

	void Framebuffer::Next()
	{
		SA_ASSERT(mRenderPassIndex < mRenderPass.size(), OutOfRange, Rendering)

		if(mRenderPassIndex != 0)
			vkCmdEndRenderPass(mPrimaryCommandBuffer);

		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = mRenderPass[mRenderPassIndex]->Get();
		renderPassBeginInfo.framebuffer = mFramebuffer;
		renderPassBeginInfo.renderArea = VkRect2D{ VkOffset2D{}, mExtent };
		renderPassBeginInfo.clearValueCount = SizeOf(mClearValues);
		renderPassBeginInfo.pClearValues = mClearValues.data();

		//vkCmdPipelineBarrier(mPrimaryCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE);

		vkCmdBeginRenderPass(mPrimaryCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

		++mRenderPassIndex;
	}

	void Framebuffer::End()
	{
		vkCmdEndRenderPass(mPrimaryCommandBuffer);

		SA_VK_ASSERT(vkEndCommandBuffer(mPrimaryCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");

		mRenderPassIndex = 0;
	}
}
