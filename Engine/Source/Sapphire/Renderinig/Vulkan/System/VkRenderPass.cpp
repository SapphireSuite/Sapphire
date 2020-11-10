// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderPass.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

namespace Sa::Vk
{
	VkSubpassDescription CreateSubpassDesc()
	{
		VkSubpassDescription subpass;

		subpass.flags = 0u;
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.inputAttachmentCount = 0u;
		subpass.pInputAttachments = nullptr;

		subpass.colorAttachmentCount = 0u;
		subpass.pColorAttachments = nullptr;
		
		subpass.pResolveAttachments = nullptr;
		subpass.pDepthStencilAttachment = nullptr;

		subpass.preserveAttachmentCount = 0u;
		subpass.pPreserveAttachments = nullptr;

		return subpass;
	}

	VkSubpassDependency CreateSubpassDep(uint32 _currIndex, uint32 _subpassNum)
	{
		VkSubpassDependency subpassDependency;

		// Default dependency.
		subpassDependency.srcSubpass = _currIndex - 1;
		subpassDependency.dstSubpass = _currIndex;

		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		// Specialization: first dependency.
		if (_currIndex == 0)
		{
			subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;

			subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		}

		// Specialization: last dependency.
		if (_currIndex == _subpassNum - 1)
		{
			if(_currIndex != 0) // Only if not also first dependency.
				subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;

			subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		}

		return subpassDependency;
	}

	VkAttachmentDescription CreateAttachement(VkFormat _format, VkSampleCountFlagBits _sampling, VkAttachmentLoadOp _loadOp)
	{
		VkAttachmentDescription attachment;
		attachment.flags = 0u;
		attachment.format = _format;
		attachment.samples = _sampling;
		attachment.loadOp = _loadOp;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilLoadOp = _loadOp;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		return attachment;
	}

	void RenderPass::Create(const IRenderInstance& _instance, const RenderPassCreateInfos& _infos)
	{
		const VkDevice& device = _instance.As<RenderInstance>().GetDevice();

		const VkSampleCountFlagBits sampling = static_cast<VkSampleCountFlagBits>(_infos.sampling);
		const VkAttachmentLoadOp loadOp = _infos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;

		// === Attachments ===
		std::vector<uint32> offsetIndex;
		offsetIndex.push_back(0u); // Add first offset.
		offsetIndex.reserve(10u);

		std::vector<VkAttachmentDescription> attachments;
		attachments.reserve(10u);

		std::vector<VkAttachmentReference> attachmentRefs;
		attachmentRefs.reserve(10u);

		std::vector<VkAttachmentReference> inputAttachmentRefs;
		inputAttachmentRefs.reserve(10u);

		VkAttachmentReference depthAttachmentRef;
		VkAttachmentReference presentAttachmentResolveRef;

		
		for (uint32 i = 0; i < SizeOf(_infos.subPassInfos); ++i)
		{
			const SubPassCreateInfos& subpassInfos = _infos.subPassInfos[i];

			for (auto attIt = subpassInfos.attachmentInfos.begin(); attIt != subpassInfos.attachmentInfos.end(); ++attIt)
			{
				attachments.push_back(CreateAttachement(API_GetRenderFormat(attIt->format), sampling, loadOp));
				attachmentRefs.push_back({ SizeOf(attachments) - 1u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
				inputAttachmentRefs.push_back({ SizeOf(attachments) - 1u, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
			}

			uint32 offset = SizeOf(attachments);
			offsetIndex.push_back(offset + offsetIndex[i]);
		}

		// Add present attachment.
		if (_infos.bPresent)
		{
			VkFormat presentFormat = API_GetRenderFormat(_infos.presentFormat);
			VkAttachmentDescription presentAttachment = CreateAttachement(presentFormat, sampling, loadOp);

			if (sampling != VK_SAMPLE_COUNT_1_BIT)
			{
				// Color attachment multisampling resolution.
				VkAttachmentDescription presentAttachmentResolve = CreateAttachement(presentFormat, VK_SAMPLE_COUNT_1_BIT, loadOp);
				presentAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

				attachments.push_back(presentAttachmentResolve);
				presentAttachmentResolveRef = VkAttachmentReference{ SizeOf(attachments) - 1u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
			}
			else
				presentAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			attachments.push_back(presentAttachment);
			attachmentRefs.push_back({ SizeOf(attachments) - 1u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

			offsetIndex.push_back(offsetIndex[SizeOf(offsetIndex) - 1] + 1); // Add one attachment (resolve not counted).
		}

		// Add Depth attachement.
		if (_infos.bDepthBuffer)
		{
			/**
			*	Use VK_FORMAT_D24_UNORM_S8_UINT or VK_FORMAT_D16_SFLOAT instead of VK_FORMAT_D32_SFLOAT optimization.
			*	Sources: https://developer.nvidia.com/blog/vulkan-dos-donts/
			*/
			VkFormat depthFormat = _infos.bStencilBuffer ? VK_FORMAT_D24_UNORM_S8_UINT : VK_FORMAT_D16_UNORM;

			VkAttachmentDescription depthAttachment = CreateAttachement(depthFormat, sampling, loadOp);
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			
			attachments.push_back(depthAttachment);

			depthAttachmentRef = VkAttachmentReference{ SizeOf(attachments) - 1u, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
		}
		
		// Split into 2 for loops to avoid bad pointers due to vector reallocation.

		// === Subpasses ===
		uint32 subpassNum = SizeOf(_infos.subPassInfos);

		// Add present subpass.
		if (_infos.bPresent)
			++subpassNum;

		std::vector<VkSubpassDescription> subpassDescriptions;
		subpassDescriptions.resize(subpassNum);

		std::vector<VkSubpassDependency> subpassDependencies;
		subpassDependencies.resize(subpassNum);

		for (uint32 i = 0; i < subpassNum; ++i)
		{
			// Description.
			VkSubpassDescription& subpassDesc = subpassDescriptions[i];

			subpassDesc = CreateSubpassDesc();

			subpassDesc.colorAttachmentCount = offsetIndex[i + 1] - offsetIndex[i];
			subpassDesc.pColorAttachments = attachmentRefs.data() + offsetIndex[i];

			if (_infos.bDepthBuffer)
				subpassDesc.pDepthStencilAttachment = &depthAttachmentRef;

			if (i > 0u)
			{
				// Add input attachment of previous subpass.
				subpassDesc.inputAttachmentCount = offsetIndex[i] - offsetIndex[i - 1];
				subpassDesc.pInputAttachments = inputAttachmentRefs.data() + offsetIndex[i - 1];
			}


			// Dependency.
			VkSubpassDependency& subpassDep = subpassDependencies[i];
			subpassDep = CreateSubpassDep(i, subpassNum);
		}


		// Add present resolve attachment.
		if (_infos.bPresent && sampling != VK_SAMPLE_COUNT_1_BIT)
			subpassDescriptions[subpassNum - 1].pResolveAttachments = &presentAttachmentResolveRef;
		

		// === RenderPass ===
		VkRenderPassCreateInfo renderPassCreateInfo;
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.pNext = nullptr;
		renderPassCreateInfo.flags = VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM;
		renderPassCreateInfo.attachmentCount = SizeOf(attachments);
		renderPassCreateInfo.pAttachments = attachments.data();
		renderPassCreateInfo.subpassCount = SizeOf(subpassDescriptions);
		renderPassCreateInfo.pSubpasses = subpassDescriptions.data();
		renderPassCreateInfo.dependencyCount = SizeOf(subpassDependencies);
		renderPassCreateInfo.pDependencies = subpassDependencies.data();

		SA_VK_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create render pass!");
	}

	void RenderPass::Destroy(const IRenderInstance& _instance)
	{
		SA_ASSERT(mHandle != VK_NULL_HANDLE, InvalidParam, Rendering, L"Try to destroyed a null RenderPass!");

		const VkDevice& device = _instance.As<RenderInstance>().GetDevice();

		vkDestroyRenderPass(device, mHandle, nullptr);
	}
}
