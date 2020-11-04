// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/System/VkRenderPass.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	VkRenderPass RenderPass::Get() const noexcept
	{
		return mRenderPass;
	}

	VkFormat RenderPass::GetColorFormat() const noexcept
	{
		return mColorFormat;
	}

	SampleBits RenderPass::GetSampleBits() const noexcept
	{
		return mSampleBits;
	}

	void RenderPass::Create(const IRenderInstance& _instance, const RenderPassCreateInfos& _createInfos)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mSampleBits = _createInfos.sampling;
		mColorFormat = _createInfos.format;

		// Color Attachement.
		const VkImageLayout finalLayout = mSampleBits > SampleBits::Sample1Bit ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		const VkAttachmentDescription colorAttachment
		{
			0,														// flags.
			_createInfos.format,									// format.
			static_cast<VkSampleCountFlagBits>(mSampleBits),		// samples.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_STORE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,				// initialLayout.
			_createInfos.bPresent ? finalLayout : VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL												// finalLayout.
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
			_createInfos.format,								// format.
			VK_SAMPLE_COUNT_1_BIT,									// samples.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			_createInfos.bPresent ? VK_IMAGE_LAYOUT_PRESENT_SRC_KHR	: VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL						// finalLayout.
		};

		const VkAttachmentReference colorAttachmentResolveRef
		{
			1,														// attachment.
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,				// layout.
		};

		// Setup G-buffer
		const VkAttachmentDescription positionAttachment
		{
			0,														// flags.
			_createInfos.format,									// format.
			static_cast<VkSampleCountFlagBits>(mSampleBits),		// samples.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL			// finalLayout.
		};

		const VkAttachmentDescription normalAttachment
		{
			0,														// flags.
			_createInfos.format,									// format.
			static_cast<VkSampleCountFlagBits>(mSampleBits),		// samples.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL			// finalLayout.
		};

		const VkAttachmentDescription albedoAttachment
		{
			0,														// flags.
			_createInfos.format,									// format.
			static_cast<VkSampleCountFlagBits>(mSampleBits),		// samples.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL			// finalLayout.
		};

		const VkAttachmentDescription depthAttachment
		{
			0,														// flags.
			VK_FORMAT_D32_SFLOAT,									// format.
			static_cast<VkSampleCountFlagBits>(mSampleBits),		// samples.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			_createInfos.bClear ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_DONT_CARE,						// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL		// finalLayout.
		};

		const VkAttachmentReference depthAttachmentRef
		{
			4,			// attachment.
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,		// layout.
		};

		const VkAttachmentReference* resolveAttachments = mSampleBits > SampleBits::Sample1Bit ? &colorAttachmentResolveRef : nullptr;
		const VkAttachmentReference* depthAttachmentRefPtr = _createInfos.bDepthBuffer ? &depthAttachmentRef : nullptr;

		// FILL G-BUFFER
		VkAttachmentReference colorReferences[4];
		colorReferences[0] = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		colorReferences[1] = { 1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		colorReferences[2] = { 2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		colorReferences[3] = { 3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		VkSubpassDescription fillSubpass{};
		fillSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		fillSubpass.colorAttachmentCount = 4;
		fillSubpass.pColorAttachments = colorReferences;
		fillSubpass.pDepthStencilAttachment = depthAttachmentRefPtr;

		// COMPOSITION G-BUFFER
		VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		VkAttachmentReference inputReferences[3];
		inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
		inputReferences[1] = { 2, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
		inputReferences[2] = { 3, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

		uint32_t preserveAttachmentIndex = 1;

		VkSubpassDescription compositionSubpass{};
		compositionSubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		compositionSubpass.colorAttachmentCount = 1;
		compositionSubpass.pColorAttachments = &colorReference;
		compositionSubpass.pDepthStencilAttachment = depthAttachmentRefPtr;
		compositionSubpass.inputAttachmentCount = 3;
		compositionSubpass.pInputAttachments = inputReferences;

		// FORWARD TRANSPARENCY
		colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };

		inputReferences[0] = { 1, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

		VkSubpassDescription forwardTransparencySubpass{};
		forwardTransparencySubpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		forwardTransparencySubpass.colorAttachmentCount = 1;
		forwardTransparencySubpass.pColorAttachments = &colorReference;
		forwardTransparencySubpass.inputAttachmentCount = 1;
		forwardTransparencySubpass.pInputAttachments = inputReferences;


		// Use subpass dependencies for layout transitions
		VkSubpassDependency dependencies[4];

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = 1;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[2].srcSubpass = 1;
		dependencies[2].dstSubpass = 2;
		dependencies[2].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[2].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		dependencies[2].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[2].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		dependencies[2].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[3].srcSubpass = 0;
		dependencies[3].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[3].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependencies[3].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[3].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependencies[3].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[3].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		std::vector<VkAttachmentDescription> attachements; //{ colorAttachment, colorAttachmentResolve, depthAttachment };
		attachements.reserve(3);

		attachements.push_back(colorAttachment);

		if(mSampleBits > SampleBits::Sample1Bit)
			attachements.push_back(colorAttachmentResolve);
		
		if(_createInfos.bDepthBuffer)
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
			2,														// dependencyCount.
			dependencies											// pDependencies.
		};

		SA_VK_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &mRenderPass),
			CreationFailed, Rendering, L"Failed to create render pass!");
	}
	
	void RenderPass::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		vkDestroyRenderPass(device, mRenderPass, nullptr);
	}

	//void VkRenderPass::ReCreate(const IRenderInstance& _instance, const IRenderSurface& _surface)
	//{
	//	Destroy(_instance);

	//	Create(_instance, _surface);
	//}

	/*void RenderPass::SetSampleCount(const VkDevice& _device, SampleBits _desiredSampling)
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
			current >>= 1;
		}

		mSampleBits = SampleBits::Sample1Bit;
	}*/
}

#endif
