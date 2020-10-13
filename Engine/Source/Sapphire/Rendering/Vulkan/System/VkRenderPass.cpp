// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderPass.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkSwapChain.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	// TODO: CLEAN LATER.
	IRenderPass* IRenderPass::mainRenderPass = nullptr;


	uint32 VkRenderPass::GetImageNum() const noexcept
	{
		return SizeOf(mFrameBuffers);
	}

	VkUniformBuffer& VkRenderPass::GetUniformBuffer(uint32 _index)
	{
		SA_ASSERT(_index < SizeOf(mStaticUniformBuffers), OutOfRange, _index, 0u, SizeOf(mStaticUniformBuffers))

		return mStaticUniformBuffers[_index];
	}

	const std::vector<VkUniformBuffer>& VkRenderPass::GetStaticUniformBuffers() const
	{
		return mStaticUniformBuffers;
	}

	void VkRenderPass::Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const RenderPassCreateInfos& _createInfos)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();
		const VkSwapChain& swapChain = _surface.As<VkRenderSurface>().GetSwapChain();

		VkSampleCountFlagBits sample = static_cast<VkSampleCountFlagBits>(_createInfos.subPassCreateInfos.sampling);
		//bool bDepthBuffer = _createInfos.subPassCreateInfos.bDepthBuffer;

		// Color Attachement.
		const VkAttachmentDescription colorAttachment
		{
			0,														// flags.
			swapChain.GetImageFormat(),								// format.
			sample,													// samples.
			VK_ATTACHMENT_LOAD_OP_CLEAR,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR							// finalLayout.
		};

		const VkAttachmentReference colorAttachmentRef
		{
			0,														// attachment.
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,				// layout.
		};


		// Depth Attachement.
		const VkAttachmentDescription depthAttachment
		{
			0,														// flags.
			VK_FORMAT_D32_SFLOAT,									// format.
			VK_SAMPLE_COUNT_1_BIT,									// samples.
			VK_ATTACHMENT_LOAD_OP_CLEAR,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL		// finalLayout.
		};


		const VkAttachmentReference depthAttachmentRef
		{
			1,														// attachment.
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,		// layout.
		};


		const VkSubpassDescription subpass
		{
			0,														// flags.
			VK_PIPELINE_BIND_POINT_GRAPHICS,						// pipelineBindPoint.
			0,														// inputAttachmentCount.
			nullptr,												// pInputAttachments.
			1,														// colorAttachmentCount.
			&colorAttachmentRef,									// pColorAttachments.
			nullptr,												// pResolveAttachments.
			&depthAttachmentRef,									// pDepthStencilAttachment.
			//bDepthBuffer ? &depthAttachmentRef : nullptr,			// pDepthStencilAttachment.
			0,														// preserveAttachmentCount.
			nullptr													// pPreserveAttachments.

		};

		const VkSubpassDependency subpassDependency
		{
			VK_SUBPASS_EXTERNAL,									// srcSubpass.
			0,														// dstSubpass.
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,			// srcStageMask.
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,			// dstStageMask.
			0,														// srcAccessMask.
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,					// dstAccessMask.
			0														// dependencyFlags.
		};


		VkAttachmentDescription attachements[]{ colorAttachment, depthAttachment };

		const VkRenderPassCreateInfo renderPassCreateInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,				// sType.
			nullptr,												// pNext.
			VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM,				// flags.
			SizeOf(attachements),									// attachmentCount.
			attachements,											// pAttachments.
			1,														// subpassCount.
			&subpass,												// pSubpasses.
			1,														// dependencyCount.
			&subpassDependency										// pDependencies.
		};

		SA_VK_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create render pass!");

		mExtent = _createInfos.extent.IsValid() ? _createInfos.extent : swapChain.GetImageExtent();

		CreateUniformBuffers(device, swapChain, _createInfos);

		CreateDepthBuffer(device, swapChain, _createInfos);
		CreateFrameBuffers(device, swapChain);
	}
	
	void VkRenderPass::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		DestroyFrameBuffers(device);
		DestroyDepthBuffer(device);
		DestroyUniformBuffers(device);

		vkDestroyRenderPass(device, mHandle, nullptr);
	}

	//void VkRenderPass::ReCreate(const IRenderInstance& _instance, const IRenderSurface& _surface)
	//{
	//	Destroy(_instance);

	//	Create(_instance, _surface);
	//}

	void VkRenderPass::CreateUniformBuffers(const VkDevice& _device, const VkSwapChain& _swapChain, const RenderPassCreateInfos& _createInfos)
	{
		uint32 imageNum = _swapChain.GetImageNum();

		mStaticUniformBuffers.resize(imageNum);

		for (uint32 i = 0; i < imageNum; ++i)
		{
			mStaticUniformBuffers[i].Create(_device, _createInfos.staticUniformBufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}
	}
	void VkRenderPass::DestroyUniformBuffers(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mStaticUniformBuffers.size(); ++i)
			mStaticUniformBuffers[i].Destroy(_device);

		mStaticUniformBuffers.clear();
	}

	void VkRenderPass::CreateDepthBuffer(const VkDevice& _device, const VkSwapChain& _swapChain, const RenderPassCreateInfos& _createInfos)
	{
		//if (!_createInfos.subPassCreateInfos.bDepthBuffer)
		//	return;

		const VkImageBufferCreateInfos imageBufferCreateInfos
		{
			VK_FORMAT_D32_SFLOAT,								// format.
			mExtent,											// extent.
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,		// usage.
			VK_IMAGE_ASPECT_DEPTH_BIT,							// aspectFlags
		};

		mDepthBuffer.Create(_device, imageBufferCreateInfos);

		mDepthBuffer.TransitionImageLayout(_device, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}
	
	void VkRenderPass::DestroyDepthBuffer(const VkDevice& _device)
	{
		if(mDepthBuffer.IsValid())
			mDepthBuffer.Destroy(_device);
	}


	void VkRenderPass::CreateFrameBuffers(const VkDevice& _device, const VkSwapChain& _swapChain)
	{
		mFrameBuffers.resize(_swapChain.GetImageNum());

		for (uint32 i = 0u; i < mFrameBuffers.size(); ++i)
		{
			const VkImageView attachements[]{ _swapChain.GetImageView(i), mDepthBuffer };

			const VkFramebufferCreateInfo framebufferCreateInfo
			{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,			// sType.
				nullptr,											// pNext.
				0,													// flags.
				mHandle,											// renderPass.
				SizeOf(attachements),								// attachmentCount.
				attachements,										// pAttachments.
				mExtent.width,										// width.
				mExtent.height,										// height.
				1													// layers.

			};

			SA_VK_ASSERT(vkCreateFramebuffer(_device, &framebufferCreateInfo, nullptr, &mFrameBuffers[i]),
				CreationFailed, Rendering, L"Failed to create framebuffer!");
		}
	}

	void VkRenderPass::DestroyFrameBuffers(const VkDevice& _device)
	{
		for (uint32 i = 0u; i < mFrameBuffers.size(); ++i)
			vkDestroyFramebuffer(_device, mFrameBuffers[i], nullptr);

		mFrameBuffers.clear();
	}

	void VkRenderPass::Begin(const IRenderFrame& _frame)
	{
		const VkRenderFrame& vkFrame = _frame.As<VkRenderFrame>();

		const VkRenderPassBeginInfo renderPassBeginInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,							// sType.
			nullptr,															// pNext.
			mHandle,															// renderPass.
			mFrameBuffers[vkFrame.index],										// framebuffer
			VkRect2D{ VkOffset2D{}, mExtent },									// renderArea.
			SizeOf(clearValue),													// clearValueCount.
			clearValue															// pClearValues.
		};

		vkCmdBeginRenderPass(vkFrame.graphicsCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VkRenderPass::End(const IRenderFrame& _frame)
	{
		const VkRenderFrame& vkFrame = _frame.As<VkRenderFrame>();

		vkCmdEndRenderPass(vkFrame.graphicsCommandBuffer);
	}

	VkRenderPass::operator ::VkRenderPass() const noexcept
	{
		return mHandle;
	}
}

#endif
