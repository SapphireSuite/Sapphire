// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex/Vertex.hpp>
#include <Rendering/Framework/Primitives/Material/RawMaterial.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>
#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderPipeline::Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		const VkRenderInstance& vkInstance = _instance.As<VkRenderInstance>();
		const VkDevice& device = vkInstance.GetDevice();
		const RenderPass& vkRenderPass = _infos.renderPass->As<RenderPass>();


		CreateDescriptors(vkInstance, _infos);

		// === Create pipeline layout ===
		std::vector<VkPushConstantRange> pushConstants;
		_infos.pushConstInfos.API_GetPushConstantRanges(pushConstants);

		const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,							// sType.
			nullptr,																// pNext.
			0,																		// flags.
			1,																		// setLayoutCount.
			&mDescriptorSetLayout,													// pSetLayouts.
			SizeOf(pushConstants),													// pushConstantRangeCount.
			pushConstants.data()													// pPushConstantRanges.
		};

		SA_VK_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout),
			CreationFailed, Rendering, L"Failed to create pipeline layout!");

		// === Create pipeline ===

		// Shader stages.
		std::vector<VkSpecConstantRange> specConstRanges;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		CreateShaderStages(shaderStages, specConstRanges, _infos);

		// Vertex inputs.
		// TODO: Fix unique_ptr free (bindingDescriptions and attributeDescriptions are unique_ptr).
		//const VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo = CreateVertexInputInfos(_infos);
		auto bindingDescriptions = _infos.vertexBindingLayout.GetBindingDescription();
		auto attributeDescriptions = _infos.vertexBindingLayout.GetAttributeDescriptions();

		const VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			_infos.vertexBindingLayout.GetBindingDescriptionNum(),					// vertexBindingDescriptionCount.
			bindingDescriptions.get(),												// pVertexBindingDescriptions.
			_infos.vertexBindingLayout.GetAttributeDescriptionNum(),				// vertexAttributeDescriptionCount.
			attributeDescriptions.get()												// pVertexAttributeDescriptions.
		};


		const VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,			// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,									// topology.
			VK_FALSE																// primitiveRestartEnable.
		};


		// Viewport.
		std::vector<VkViewport> viewports; std::vector<VkRect2D> scissors;
		const VkPipelineViewportStateCreateInfo viewportStateCreateInfo = CreateViewportStateCreateInfos(_infos, viewports, scissors);

		VkDynamicState dynamicState[]{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		const VkPipelineDynamicStateCreateInfo DynamiCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,					// sType.
			nullptr,																// pNext.
			0,																		// flags.
			SizeOf(dynamicState),													// dynamicStateCount.
			dynamicState															// pDynamicStates.
		};


		// Rasterizer.
		const VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo = CreateRasterizationInfos(_infos);

		// Multisampling.
		const VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			static_cast<VkSampleCountFlagBits>(vkRenderPass.GetSampleBits()),		// rasterizationSamples.
			VK_TRUE,																// sampleShadingEnable.
			0.2f,																	// minSampleShading.
			nullptr,																// pSampleMask.
			VK_FALSE,																// alphaToCoverageEnable.
			VK_FALSE																// alphaToOneEnable.
		};

		// Depth stencil.
		const VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_TRUE,																// depthTestEnable.
			VK_TRUE,																// depthWriteEnable.
			VK_COMPARE_OP_LESS_OR_EQUAL,											// depthCompareOp.
			VK_FALSE,																// depthBoundsTestEnable.
			VK_FALSE,																// stencilTestEnable.
			{},																		// front.
			{},																		// back.
			0.0f,																	// minDepthBounds.
			1.0f,																	// maxDepthBounds.
		};

		// Color attachement.
		const VkPipelineColorBlendAttachmentState colorBlendAttachment
		{
			VK_TRUE,																// blendEnable.
			VK_BLEND_FACTOR_SRC_ALPHA,												// srcColorBlendFactor.
			VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,									// dstColorBlendFactor.
			VK_BLEND_OP_ADD,														// colorBlendOp.
			VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,									// srcAlphaBlendFactor.
			VK_BLEND_FACTOR_ZERO,													// dstAlphaBlendFactor.
			VK_BLEND_OP_ADD,														// alphaBlendOp.

			VK_COLOR_COMPONENT_R_BIT |												// colorWriteMask.
			VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT
		};


		const VkPipelineColorBlendStateCreateInfo colorBlendingCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_FALSE,																// logicOpEnable.
			VK_LOGIC_OP_COPY,														// logicOp.
			1,																		// attachmentCount.
			&colorBlendAttachment,													// pAttachments.
			{0.0f, 0.0f, 0.0f, 0.0f}												// blendConstants[4].
		};


		const VkGraphicsPipelineCreateInfo pipelineCreateInfo
		{
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,						// sType.
			nullptr,																// pNext.
			0,																		// flags.
			SizeOf(shaderStages),													// stageCount.
			shaderStages.data(),													// pStages.
			&vertexInputCreateInfo,													// pVertexInputState.
			&inputAssemblyCreateInfo,												// pInputAssemblyState.
			nullptr,																// pTessellationState.
			&viewportStateCreateInfo,												// pViewportState.
			&rasterizerCreateInfo,													// pRasterizationState.
			&multisamplingCreateInfo,												// pMultisampleState.
			&depthStencilCreateInfo,												// pDepthStencilState.
			&colorBlendingCreateInfo,												// pColorBlendState.
			_infos.bDynamicViewport ? &DynamiCreateInfo : nullptr,					// pDynamicState.
			mPipelineLayout,														// layout.
			vkRenderPass.Get(),														// renderPass.
			0,																		// subpass.
			VK_NULL_HANDLE,															// basePipelineHandle.
			-1																		// basePipelineIndex.
		};

		SA_VK_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create graphics pipeline!");
	}

	void VkRenderPipeline::CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
		std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos)
	{
		_shaderStages.reserve(_infos.shaders.size());
		_specConstRanges.reserve(_infos.shaders.size());

		// TODO: REMOVE LATER.
		static int specs[2];
		specs[0] = static_cast<int>(_infos.alphaModel);
		specs[1] = static_cast<int>(_infos.pipelineFlags);


		for (auto it = _infos.shaders.begin(); it != _infos.shaders.end(); ++it)
		{
			VkSpecConstantRange* specConstRange = nullptr;

			if (SizeOf(it->specConstants) > 0)
				specConstRange = &it->specConstants.API_GetSpecConstantRanges(_specConstRanges, specs);

			_shaderStages.emplace_back(VkPipelineShaderStageCreateInfo
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
				nullptr,															// pNext.
				0,																	// flags.
				API_GetShaderType(it->type),										// stage.
				it->shader->As<VkShader>(),											// module.
				"main",																// pName.
				specConstRange ? &specConstRange->infos : nullptr,					// pSpecializationInfo.
			});
		}
	}

	VkPipelineViewportStateCreateInfo VkRenderPipeline::CreateViewportStateCreateInfos(const PipelineCreateInfos& _infos,
		std::vector<VkViewport>& _viewports, std::vector<VkRect2D>& _scissors)
	{
		//uint32 num = _infos.cameras.size();

		//if (_infos.bDynamicViewport)
		//{
		//	return VkPipelineViewportStateCreateInfo
		//	{
		//		VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,					// sType.
		//		nullptr,																// pNext.
		//		0,																		// flags.
		//		num,																	// viewportCount.
		//		nullptr,																// pViewports.
		//		num,																	// scissorCount.
		//		nullptr																	// pScissors.
		//	};
		//}


		//// Static viewport and scissors.
		//_viewports.reserve(num);
		//_scissors.reserve(num);

		//for (auto it = _infos.cameras.begin(); it != _infos.cameras.end(); ++it)
		//{
		//	_viewports.push_back((*it)->GetVkViewport());
		//	_scissors.push_back((*it)->GetVkScissor());
		//}

		//return VkPipelineViewportStateCreateInfo
		//{
		//	VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,					// sType.
		//	nullptr,																// pNext.
		//	0,																		// flags.
		//	num,																	// viewportCount.
		//	_viewports.data(),														// pViewports.
		//	num,																	// scissorCount.
		//	_scissors.data()														// pScissors.
		//};


		_viewports.push_back(VkViewport
		{
			0.0f,								// x.
			0.0f,								// y.
			1200.0f,							// width.
			800.0f,								// height.
			0.0f,								// minDepth.
			1.0f,								// maxDepth.
		});

		_scissors.push_back(VkRect2D{
			VkOffset2D{ 0, 0 },
			VkExtent2D{ 1200, 800 }
		});

		return VkPipelineViewportStateCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,					// sType.
			nullptr,																// pNext.
			0,																		// flags.
			1,																		// viewportCount.
			_viewports.data(),														// pViewports.
			1,																		// scissorCount.
			_scissors.data()														// pScissors.
		};
	}

	//VkPipelineVertexInputStateCreateInfo VkRenderPipeline::CreateVertexInputInfos(const PipelineCreateInfos& _infos)
	//{
	//	auto bindingDescriptions = _infos.vertexBindingLayout.GetBindingDescription();
	//	auto attributeDescriptions = _infos.vertexBindingLayout.GetAttributeDescriptions();

	//	return VkPipelineVertexInputStateCreateInfo
	//	{
	//		VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,				// sType.
	//		nullptr,																// pNext.
	//		0,																		// flags.
	//		_infos.vertexBindingLayout.GetBindingDescriptionNum(),					// vertexBindingDescriptionCount.
	//		bindingDescriptions.get(),												// pVertexBindingDescriptions.
	//		_infos.vertexBindingLayout.GetAttributeDescriptionNum(),				// vertexAttributeDescriptionCount.
	//		attributeDescriptions.get()												// pVertexAttributeDescriptions.
	//	};
	//}

	VkPipelineRasterizationStateCreateInfo VkRenderPipeline::CreateRasterizationInfos(const PipelineCreateInfos& _infos)
	{
		return VkPipelineRasterizationStateCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_FALSE,																// depthClampEnable.
			VK_FALSE,																// rasterizerDiscardEnable.
			API_GetPolygonMode(_infos.polygonMode),									// polygonMode.
			API_GetCullingMode(_infos.cullingMode),									// cullMode.
			API_GetFrontFaceMode(_infos.frontFaceMode),								// frontFace.
			VK_FALSE,																// depthBiasEnable.
			0.0f,																	// depthBiasConstantFactor.
			0.0f,																	// depthBiasClamp.
			0.0f,																	// depthBiasSlopeFactor.
			1.0f																	// lineWidth.
		};
	}

	void VkRenderPipeline::CreateDescriptors(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		CreateDescriptorSetLayout(_instance, _infos);

		const uint32 imageNum = 3; // TODO Aurel: Remove hardcoded image number

		CreateDescriptorPool(_instance, _infos, imageNum);
		CreateDescriptorSets(_instance, _infos, imageNum);
	}
	void VkRenderPipeline::DestroyDescriptors(const VkDevice& _device)
	{
		// Not needed when vkDestroyDescriptorPool() is called. Requiere VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT flag.
		//vkFreeDescriptorSets(_device, mDescriptorPool, static_cast<uint32>(mDescriptorSets.size()), mDescriptorSets.data());
		mDescriptorSets.clear();

		vkDestroyDescriptorSetLayout(_device, mDescriptorSetLayout, nullptr);
		mDescriptorSetLayout = VK_NULL_HANDLE;

		vkDestroyDescriptorPool(_device, mDescriptorPool, nullptr);
		mDescriptorPool = VK_NULL_HANDLE;
	}


	void VkRenderPipeline::Create(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		Create_Internal(_instance, _infos);

		// TODO: CLEAN.
		//_infos.surface.onResizeEvent.Add(std::function<void(const IRenderInstance&, const IRenderSurface&)>(
		//	[this, &_infos](const IRenderInstance& _instance, const IRenderSurface& _surface)
		//	{
		//		(void)_surface;
		//		ReCreate(_instance, _infos);
		//	}
		//));
	}

	void VkRenderPipeline::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		DestroyDescriptors(device);

		vkDestroyPipeline(device, mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;

		vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);
		mPipelineLayout = VK_NULL_HANDLE;
	}

	void VkRenderPipeline::Bind(const IRenderFrame& _frame) const
	{
		const VkRenderFrame& vkFrame = _frame.As<VkRenderFrame>();

		vkCmdBindPipeline(vkFrame.framebuffer->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mHandle);

		vkCmdBindDescriptorSets(vkFrame.framebuffer->GetCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout,
			0, 1, &mDescriptorSets[vkFrame.index], 0, nullptr);
	}

	void VkRenderPipeline::ReCreate(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		Destroy(_instance);

		Create_Internal(_instance, _infos);
	}


	VkRenderPipeline::operator VkPipeline() const
	{
		return mHandle;
	}

	VkRenderPipeline::operator VkPipelineLayout() const
	{
		return mPipelineLayout;
	}
}

#endif
