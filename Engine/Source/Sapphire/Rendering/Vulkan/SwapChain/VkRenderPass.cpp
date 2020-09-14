// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/SwapChain/VkRenderPass.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/VkDevice.hpp>
#include <Rendering/Vulkan/SwapChain/VkSwapChain.hpp>

namespace Sa
{
	void VkRenderPass::Create(const VkDevice& _device, const VkSwapChain& _swapChain)
	{
		const VkAttachmentDescription colorAttachment
		{
			0,														// flags.
			_swapChain.GetImageFormat(),							// format.
			VK_SAMPLE_COUNT_1_BIT,									// samples.
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


		const VkSubpassDescription subpass
		{
			0,														// flags.
			VK_PIPELINE_BIND_POINT_GRAPHICS,						// pipelineBindPoint.
			0,														// inputAttachmentCount.
			nullptr,												// pInputAttachments.
			1,														// colorAttachmentCount.
			&colorAttachmentRef,									// pColorAttachments.
			nullptr,												// pResolveAttachments.
			nullptr,												// pDepthStencilAttachment.
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


		const VkRenderPassCreateInfo renderPassCreateInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,				// sType.
			nullptr,												// pNext.
			VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM,				// flags.
			1,														// attachmentCount.
			&colorAttachment,										// pAttachments.
			1,														// subpassCount.
			&subpass,												// pSubpasses.
			1,														// dependencyCount.
			&subpassDependency										// pDependencies.
		};

		SA_VK_ASSERT(vkCreateRenderPass(_device, &renderPassCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create render pass!");
	}
	void VkRenderPass::Destroy(const VkDevice& _device)
	{
		vkDestroyRenderPass(_device, mHandle, nullptr);
	}

	VkRenderPass::operator ::VkRenderPass() const noexcept
	{
		return mHandle;
	}
}
