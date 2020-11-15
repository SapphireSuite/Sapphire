// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderPass.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

namespace Sa::Vk
{
	VkSubpassDescription CreateSubpassDesc()
	{
		VkSubpassDescription subpass{};

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
		VkSubpassDependency subpassDependency{};

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
			subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;

			subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		}

		return subpassDependency;
	}

	VkAttachmentDescription CreateAttachement(VkFormat _format, VkSampleCountFlagBits _sampling, VkAttachmentLoadOp _loadOp)
	{
		VkAttachmentDescription attachment{};
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

	void RenderPass::Create(const IRenderInstance& _instance, const RenderPassDescriptor& _descriptor)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		const VkAttachmentLoadOp loadOp = _descriptor.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE;


		// === Subpasses ===
		uint32 subpassNum = SizeOf(_descriptor.subPassDescs);

		std::vector<VkSubpassDescription> subpassDescriptions;
		subpassDescriptions.reserve(subpassNum);

		std::vector<VkSubpassDependency> subpassDependencies;
		subpassDependencies.reserve(subpassNum);


		std::vector<VkAttachmentDescription> subpassAttachments;
		subpassAttachments.reserve(subpassNum * 10u);

		std::vector<std::vector<VkAttachmentReference>> subpassColorAttachmentRefs;
		subpassColorAttachmentRefs.resize(subpassNum);

		std::vector<std::vector<VkAttachmentReference>> subpassAttachmentResolveRefs;
		subpassAttachmentResolveRefs.resize(subpassNum);

		std::vector<std::vector<VkAttachmentReference>> subpassInputAttachmentRefs;
		subpassInputAttachmentRefs.resize(subpassNum);

		std::vector<VkAttachmentReference> subpassDepthAttachmentRefs;
		subpassDepthAttachmentRefs.reserve(subpassNum);


		for (uint32 i = 0; i < SizeOf(_descriptor.subPassDescs); ++i)
		{
			const SubPassDescriptor& subpassDesc = _descriptor.subPassDescs[i];
			const VkSampleCountFlagBits sampling = API_GetSampleCount(subpassDesc.sampling);

			// === Attachments ===
			std::vector<VkAttachmentReference>& colorAttachmentRefs = subpassColorAttachmentRefs[i];
			colorAttachmentRefs.reserve(subpassDesc.attachmentDescs.size());

			std::vector<VkAttachmentReference>& resolveAttachmentRefs = subpassAttachmentResolveRefs[i];
			resolveAttachmentRefs.reserve(subpassDesc.attachmentDescs.size());

			std::vector<VkAttachmentReference>& inputAttachmentRefs = subpassInputAttachmentRefs[i];
			inputAttachmentRefs.reserve(subpassDesc.attachmentDescs.size());

			VkAttachmentReference& depthAttachRef = subpassDepthAttachmentRefs.emplace_back(VkAttachmentReference{ VK_ATTACHMENT_UNUSED,
				VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });


			for (auto attIt = subpassDesc.attachmentDescs.begin(); attIt != subpassDesc.attachmentDescs.end(); ++attIt)
			{
				const VkFormat format = API_GetFormat(attIt->format);

				VkAttachmentDescription& attachDesc = subpassAttachments.emplace_back(CreateAttachement(format, sampling, loadOp));

				if (IsDepthFormat(attIt->format))
				{
					attachDesc.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					depthAttachRef.attachment = SizeOf(subpassAttachments) - 1u;
				}
				else
				{
					colorAttachmentRefs.push_back({ SizeOf(subpassAttachments) - 1u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
					resolveAttachmentRefs.push_back(VkAttachmentReference{ VK_ATTACHMENT_UNUSED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });

					if (sampling != VK_SAMPLE_COUNT_1_BIT)
					{
						// Color attachment multisampling resolution.

						VkAttachmentDescription& resolveAttachDesc = subpassAttachments.emplace_back(CreateAttachement(format, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_DONT_CARE));

						if (IsPresentFormat(attIt->format))
							resolveAttachDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

						resolveAttachmentRefs.back().attachment = SizeOf(subpassAttachments) - 1u;
					}
					else if (IsPresentFormat(attIt->format))
						attachDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				}

				if(attIt->bInputNext)
					inputAttachmentRefs.push_back({ SizeOf(subpassAttachments) - 1u, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL });
			}


			// Subpass description.
			VkSubpassDescription& vkSubpassDesc = subpassDescriptions.emplace_back(CreateSubpassDesc());

			vkSubpassDesc.colorAttachmentCount = SizeOf(colorAttachmentRefs);
			vkSubpassDesc.pColorAttachments = colorAttachmentRefs.data();
			vkSubpassDesc.pResolveAttachments = resolveAttachmentRefs.data();

			vkSubpassDesc.pDepthStencilAttachment = &depthAttachRef;

			if (i > 0u && SizeOf(subpassInputAttachmentRefs[i - 1]) > 0u)
			{
				// Add input attachment from previous subpass.
				vkSubpassDesc.inputAttachmentCount = SizeOf(subpassInputAttachmentRefs[i - 1]);
				vkSubpassDesc.pInputAttachments = subpassInputAttachmentRefs[i - 1].data();
			}


			// Subpass dependency.
			/* VkSubpassDependency& subpassDep =*/ subpassDependencies.emplace_back(CreateSubpassDep(i, subpassNum));
		}


		// === RenderPass ===
		VkRenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.pNext = nullptr;
		renderPassCreateInfo.flags = VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM;
		renderPassCreateInfo.attachmentCount = SizeOf(subpassAttachments);
		renderPassCreateInfo.pAttachments = subpassAttachments.data();
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

		const Device& device = _instance.As<RenderInstance>().device;

		vkDestroyRenderPass(device, mHandle, nullptr);
	}


	RenderPass::operator VkRenderPass() const noexcept
	{
		return mHandle;
	}
}
