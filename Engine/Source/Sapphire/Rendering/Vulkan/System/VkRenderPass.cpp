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

	SampleBits VkRenderPass::GetSampleBits() const noexcept
	{
		return mSampleBits;
	}

	void VkRenderPass::SetClearColor(const Color& _color)
	{
		if (mClearValues.size() == 0u)
		{
			mClearValues.reserve(3);
			mClearValues.resize(1);
		}

		mClearValues[0] = VkClearValue{ _color.r, _color.g, _color.b, _color.a };

		// Multisampling clear value.
		if (mClearValues.size() == 3)
			mClearValues[1] = mClearValues[0];
	}

	void VkRenderPass::Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const RenderPassCreateInfos& _createInfos)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();
		const VkSwapChain& swapChain = _surface.As<VkRenderSurface>().GetSwapChain();

		SetClearColor(_createInfos.clearColor);

		SetSampleCount(device, _createInfos.sampling);
		//bool bDepthBuffer = _createInfos.subPassCreateInfos.bDepthBuffer;

		// Color Attachement.
		const VkImageLayout finalLayout = mSampleBits > SampleBits::Sample1Bit ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		const VkAttachmentDescription colorAttachment
		{
			0,														// flags.
			swapChain.GetImageFormat(),								// format.
			static_cast<VkSampleCountFlagBits>(mSampleBits),		// samples.
			VK_ATTACHMENT_LOAD_OP_CLEAR,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			finalLayout												// finalLayout.
		};

		const VkAttachmentReference colorAttachmentRef
		{
			0,														// attachment.
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,				// layout.
		};


		// Color attachment multisampling resolution.
		const VkAttachmentDescription colorAttachmentResolve
		{
			0,														// flags.
			swapChain.GetImageFormat(),								// format.
			VK_SAMPLE_COUNT_1_BIT,									// samples.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR							// finalLayout.
		};

		const VkAttachmentReference colorAttachmentResolveRef
		{
			1,														// attachment.
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,				// layout.
		};


		// Depth Attachement.
		const VkAttachmentDescription depthAttachment
		{
			0,														// flags.
			VK_FORMAT_D32_SFLOAT,									// format.
			static_cast<VkSampleCountFlagBits>(mSampleBits),		// samples.
			VK_ATTACHMENT_LOAD_OP_CLEAR,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL		// finalLayout.
		};


		const VkAttachmentReference depthAttachmentRef
		{
			mSampleBits > SampleBits::Sample1Bit ? 2u : 1u,			// attachment.
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,		// layout.
		};


		const VkAttachmentReference* resolveAttachments = mSampleBits > SampleBits::Sample1Bit ? &colorAttachmentResolveRef : nullptr;
		
		const VkSubpassDescription subpass
		{
			0,														// flags.
			VK_PIPELINE_BIND_POINT_GRAPHICS,						// pipelineBindPoint.
			0,														// inputAttachmentCount.
			nullptr,												// pInputAttachments.
			1,														// colorAttachmentCount.
			&colorAttachmentRef,									// pColorAttachments.
			resolveAttachments,										// pResolveAttachments.
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


		std::vector<VkAttachmentDescription> attachements; //{ colorAttachment, colorAttachmentResolve, depthAttachment };
		attachements.reserve(3);

		attachements.push_back(colorAttachment);

		if(mSampleBits > SampleBits::Sample1Bit)
			attachements.push_back(colorAttachmentResolve);
		
		attachements.push_back(depthAttachment);

		const VkRenderPassCreateInfo renderPassCreateInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,				// sType.
			nullptr,												// pNext.
			VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM,				// flags.
			SizeOf(attachements),									// attachmentCount.
			attachements.data(),									// pAttachments.
			1,														// subpassCount.
			&subpass,												// pSubpasses.
			1,														// dependencyCount.
			&subpassDependency										// pDependencies.
		};

		SA_VK_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create render pass!");

		mExtent = _createInfos.extent.IsValid() ? _createInfos.extent : swapChain.GetImageExtent();

		if (mSampleBits > SampleBits::Sample1Bit)
			CreateColorMultisamplingBuffer(device, swapChain);

		CreateDepthBuffer(device, swapChain);
		CreateFrameBuffers(device, swapChain);
	}
	
	void VkRenderPass::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		DestroyFrameBuffers(device);
		DestroyDepthBuffer(device);

		DestroyColorMultisamplingBuffer(device);

		vkDestroyRenderPass(device, mHandle, nullptr);
	}

	//void VkRenderPass::ReCreate(const IRenderInstance& _instance, const IRenderSurface& _surface)
	//{
	//	Destroy(_instance);

	//	Create(_instance, _surface);
	//}

	void VkRenderPass::CreateDepthBuffer(const VkDevice& _device, const VkSwapChain& _swapChain)
	{
		//if (!_createInfos.subPassCreateInfos.bDepthBuffer)
		//	return;

		// Depth clear value.
		mClearValues.emplace_back(VkClearValue{ 1.0f, 0 });


		const VkImageBufferCreateInfos imageBufferCreateInfos
		{
			VK_FORMAT_D32_SFLOAT,								// format.
			mExtent,											// extent.
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,		// usage.
			
			1,													// mipMapLevels.
			static_cast<VkSampleCountFlagBits>(mSampleBits),	// sampleCount.

			VK_IMAGE_ASPECT_DEPTH_BIT,							// aspectFlags
		};

		mDepthBuffer.Create(_device, imageBufferCreateInfos);

		
		const VkTransitionImageInfos depthTransitionInfos
		{
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,

			1u,
			VK_IMAGE_ASPECT_DEPTH_BIT
		};

		mDepthBuffer.TransitionImageLayout(_device, depthTransitionInfos);
	}
	
	void VkRenderPass::DestroyDepthBuffer(const VkDevice& _device)
	{
		if(mDepthBuffer.IsValid())
			mDepthBuffer.Destroy(_device);
	}


	void VkRenderPass::CreateColorMultisamplingBuffer(const VkDevice& _device, const VkSwapChain& _swapChain)
	{
		// Duplicate color value.
		mClearValues.emplace_back(mClearValues[0]);

		const VkImageBufferCreateInfos colorBufferCreateInfos
		{
			_swapChain.GetImageFormat(),						// format.
			mExtent,											// extent.

			 VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |			// usage.
			 VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,

			1,													// mipMapLevels.
			static_cast<VkSampleCountFlagBits>(mSampleBits),	// sampleCount.

			VK_IMAGE_ASPECT_COLOR_BIT							// aspectFlags.
		};

		mColorMultisamplingBuffer.Create(_device, colorBufferCreateInfos);
	}

	void VkRenderPass::DestroyColorMultisamplingBuffer(const VkDevice& _device)
	{
		mColorMultisamplingBuffer.Destroy(_device);
	}


	void VkRenderPass::CreateFrameBuffers(const VkDevice& _device, const VkSwapChain& _swapChain)
	{
		mFrameBuffers.resize(_swapChain.GetImageNum());

		std::vector<VkImageView> attachements; // { mColorMultisamplingBuffer, _swapChain.GetImageView(i), mDepthBuffer }
		attachements.reserve(3);

		if (mSampleBits > SampleBits::Sample1Bit)
			attachements.push_back(mColorMultisamplingBuffer);

		// Swapchain image view.
		VkImageView& swapChainView = attachements.emplace_back(VkImageView{});

		// if(bDepthBuffer)
			attachements.push_back(mDepthBuffer);

		for (uint32 i = 0u; i < mFrameBuffers.size(); ++i)
		{
			swapChainView = _swapChain.GetImageView(i);

			const VkFramebufferCreateInfo framebufferCreateInfo
			{
				VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,			// sType.
				nullptr,											// pNext.
				0,													// flags.
				mHandle,											// renderPass.
				SizeOf(attachements),								// attachmentCount.
				attachements.data(),								// pAttachments.
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
			SizeOf(mClearValues),												// clearValueCount.
			mClearValues.data()													// pClearValues.
		};

		vkCmdBeginRenderPass(vkFrame.graphicsCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VkRenderPass::End(const IRenderFrame& _frame)
	{
		const VkRenderFrame& vkFrame = _frame.As<VkRenderFrame>();

		vkCmdEndRenderPass(vkFrame.graphicsCommandBuffer);
	}

	void VkRenderPass::SetSampleCount(const VkDevice& _device, SampleBits _desiredSampling)
	{
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(_device, &physicalDeviceProperties);

		// TODO: use limits.framebufferDepthSampleCounts only with depth buffer.
		VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

		uint8 current = static_cast<uint8>(_desiredSampling);

		while (current > 1)
		{
			if (counts & current)
			{
				mSampleBits = static_cast<SampleBits>(current);
				return;
			}

			// Sample not supported by hardware: down sample.
			current >> 1;
		}

		mSampleBits = SampleBits::Sample1Bit;
	}

	VkRenderPass::operator ::VkRenderPass() const noexcept
	{
		return mHandle;
	}
}

#endif
