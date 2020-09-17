// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/VkMacro.hpp>

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#include <Rendering/Model/Vertex.hpp>

#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Primitives/VkRenderSurface.hpp>
#include <Rendering/Vulkan/Primitives/Pipeline/VkShader.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	VkShaderStageFlagBits GetVkShaderStage(ShaderType _shaderType)
	{
		switch (_shaderType)
		{
			case ShaderType::Fragment:
				return VK_SHADER_STAGE_FRAGMENT_BIT;
			case ShaderType::Geometry:
				return VK_SHADER_STAGE_GEOMETRY_BIT;
			case ShaderType::Compute:
				return VK_SHADER_STAGE_COMPUTE_BIT;
			case ShaderType::Vertex:
			case ShaderType::Unknown:
			default:
				return VK_SHADER_STAGE_VERTEX_BIT;
		}
	}


	void VkRenderPipeline::Create_Internal(const IRenderInstance& _instance,
		const IRenderSurface& _surface,
		const std::vector<const IShader*>& _shaders,
		const Viewport& _viewport)
	{
		// Create Shader Stages.
		VkPipelineShaderStageCreateInfo* shaderStages = new VkPipelineShaderStageCreateInfo[_shaders.size()];

		for (uint32 i = 0u; i < _shaders.size(); ++i)
		{
			shaderStages[i] = VkPipelineShaderStageCreateInfo
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
				nullptr,															// pNext.
				0,																	// flags.
				GetVkShaderStage(_shaders[i]->GetShaderType()),						// stage.
				*reinterpret_cast<const VkShader*>(_shaders[i]),					// module.
				"main",																// pName.
				nullptr,															// pSpecializationInfo.
			};
		}


		const VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,			// sType.
			nullptr,															// pNext.
			0,																	// flags.
			Vertex::bindingDescriptionNum,										// vertexBindingDescriptionCount.
			Vertex::GetBindingDescription(),									// pVertexBindingDescriptions.
			Vertex::attributeDescriptionNum,									// vertexAttributeDescriptionCount.
			Vertex::GetAttributeDescriptions()									// pVertexAttributeDescriptions.
		};

		const VkPipelineInputAssemblyStateCreateInfo inputAssemblyCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,			// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,									// topology.
			VK_FALSE																// primitiveRestartEnable.
		};


		// Create Viewport.
		const VkViewport viewport = _viewport.GetVkViewport();
		const VkRect2D scissor = _viewport.GetVkScissor();

		const VkPipelineViewportStateCreateInfo viewportStateCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,					// sType.
			nullptr,																// pNext.
			0,																		// flags.
			1,																		// viewportCount.
			&viewport,																// pViewports.
			1,																		// scissorCount.
			&scissor																// pScissors.
		};


		const VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_FALSE,																// depthClampEnable.
			VK_FALSE,																// rasterizerDiscardEnable.
			VK_POLYGON_MODE_FILL,													// polygonMode.
			VK_CULL_MODE_BACK_BIT,													// cullMode.
			VK_FRONT_FACE_CLOCKWISE,												// frontFace.
			VK_FALSE,																// depthBiasEnable.
			0.0f,																	// depthBiasConstantFactor.
			0.0f,																	// depthBiasClamp.
			0.0f,																	// depthBiasSlopeFactor.
			1.0f																	// lineWidth.
		};

		const VkPipelineMultisampleStateCreateInfo multisamplingCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_SAMPLE_COUNT_1_BIT,													// rasterizationSamples.
			VK_FALSE,																// sampleShadingEnable.
			1.0,																	// minSampleShading.
			nullptr,																// pSampleMask.
			VK_FALSE,																// alphaToCoverageEnable.
			VK_FALSE																// alphaToOneEnable.
		};

		const VkPipelineColorBlendAttachmentState colorBlendAttachment
		{
			VK_TRUE,																// blendEnable.
			VK_BLEND_FACTOR_SRC_ALPHA,												// srcColorBlendFactor.
			VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,									// dstColorBlendFactor.
			VK_BLEND_OP_ADD,														// colorBlendOp.
			VK_BLEND_FACTOR_ONE,													// srcAlphaBlendFactor.
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
			VK_TRUE,																// logicOpEnable.
			VK_LOGIC_OP_COPY,														// logicOp.
			1,																		// attachmentCount.
			&colorBlendAttachment,													// pAttachments.
			{0.0f, 0.0f, 0.0f, 0.0f}												// blendConstants[4].
		};



		const VkPipelineLayoutCreateInfo  pipelineLayoutCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,							// sType.
			nullptr,																// pNext.
			0,																		// flags.
			0,																		// setLayoutCount.
			nullptr,																// pSetLayouts.
			0,																		// pushConstantRangeCount.
			nullptr																	// pPushConstantRanges.
		};

		const VkDevice& device = reinterpret_cast<const VkRenderInstance&>(_instance).GetDevice();
		SA_VK_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &mLayout),
			CreationFailed, Rendering, L"Failed to create pipeline layout!");

		const VkRenderPass& renderPass = reinterpret_cast<const VkRenderSurface&>(_surface).GetRenderPass();

		const VkGraphicsPipelineCreateInfo pipelineCreateInfo
		{
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,					// sType.
			nullptr,															// pNext.
			0,																	// flags.
			static_cast<uint32>(_shaders.size()),								// stageCount.
			shaderStages,														// pStages.
			&vertexInputCreateInfo,												// pVertexInputState.
			&inputAssemblyCreateInfo,											// pInputAssemblyState.
			nullptr,															// pTessellationState.
			&viewportStateCreateInfo,											// pViewportState.
			&rasterizerCreateInfo,												// pRasterizationState.
			&multisamplingCreateInfo,											// pMultisampleState.
			nullptr,															// pDepthStencilState.
			&colorBlendingCreateInfo,											// pColorBlendState.
			nullptr,															// pDynamicState.
			mLayout,															// layout.
			renderPass,															// renderPass.
			0,																	// subpass.
			VK_NULL_HANDLE,														// basePipelineHandle.
			-1																	// basePipelineIndex.
		};

		SA_VK_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create graphics pipeline!");
	}

	void VkRenderPipeline::Create(const IRenderInstance& _instance,
		const IRenderSurface& _surface,
		const std::vector<const IShader*>& _shaders,
		const Viewport& _viewport)
	{
		Create_Internal(_instance, _surface, _shaders, _viewport);

		_surface.onResizeEvent.Add(std::function<void(const IRenderInstance&, const IRenderSurface&)>(
			[this, _shaders](const IRenderInstance& _instance, const IRenderSurface& _surface)
			{
				ReCreate(_instance, _surface, _shaders, _surface.GetViewport());
			}
		));
	}

	void VkRenderPipeline::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = reinterpret_cast<const VkRenderInstance&>(_instance).GetDevice();

		vkDestroyPipeline(device, mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;

		vkDestroyPipelineLayout(device, mLayout, nullptr);
		mLayout = VK_NULL_HANDLE;
	}

	void VkRenderPipeline::ReCreate(const IRenderInstance& _instance,
		const IRenderSurface& _surface,
		const std::vector<const IShader*>& _shaders,
		const Viewport& _viewport)
	{
		Destroy(_instance);

		Create_Internal(_instance, _surface, _shaders, _viewport);
	}

	VkRenderPipeline::operator VkPipeline() const
	{
		return mHandle;
	}
}

#endif
