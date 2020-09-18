// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#include <Rendering/Framework/Model/Vertex.hpp>
#include <Rendering/Framework/Model/UniformBufferObject.hpp>
#include <Rendering/Framework/Primitives/Pipeline/ShaderType.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

#include <Rendering/Vulkan/VkMacro.hpp>
#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Primitives/VkRenderSurface.hpp>
#include <Rendering/Vulkan/Primitives/Pipeline/VkShader.hpp>
#include <Rendering/Vulkan/Model/VkTexture.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderPipeline::Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();
		const VkRenderSurface& vkSurface = _pipelineInfos.surface.As<VkRenderSurface>();

		CreateDescriptors(device, _pipelineInfos);


		// === Create Shader Stages ===
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		// Create vertex shader stage.
		if (_pipelineInfos.vertexShader)
		{
			shaderStages.push_back(VkPipelineShaderStageCreateInfo
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
				nullptr,															// pNext.
				0,																	// flags.
				API_GetShaderType(ShaderType::Vertex),								// stage.
				_pipelineInfos.vertexShader->As<VkShader>(),						// module.
				"main",																// pName.
				nullptr,															// pSpecializationInfo.
			});
		}

		// Create fragment shader stage.
		if (_pipelineInfos.fragementShader)
		{
			shaderStages.push_back(VkPipelineShaderStageCreateInfo
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
				nullptr,															// pNext.
				0,																	// flags.
				API_GetShaderType(ShaderType::Fragment),							// stage.
				_pipelineInfos.fragementShader->As<VkShader>(),						// module.
				"main",																// pName.
				nullptr,															// pSpecializationInfo.
			});
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


		// === Create Viewport ===
		const VkViewport viewport = _pipelineInfos.viewport.GetVkViewport();
		const VkRect2D scissor = _pipelineInfos.viewport.GetVkScissor();

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


		// === Create Resterizer ===
		const VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_FALSE,																// depthClampEnable.
			VK_FALSE,																// rasterizerDiscardEnable.
			API_GetPolygonMode(_pipelineInfos.polygonMode),							// polygonMode.
			API_GetCullingMode(_pipelineInfos.cullingMode),							// cullMode.
			API_GetFrontFaceMode(_pipelineInfos.frontFaceMode),						// frontFace.
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


		const VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,							// sType.
			nullptr,																// pNext.
			0,																		// flags.
			1,																		// setLayoutCount.
			&mDescriptorSetLayout,													// pSetLayouts.
			0,																		// pushConstantRangeCount.
			nullptr																	// pPushConstantRanges.
		};

		SA_VK_ASSERT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout),
			CreationFailed, Rendering, L"Failed to create pipeline layout!");


		const VkRenderPass& renderPass = vkSurface.GetRenderPass();

		const VkGraphicsPipelineCreateInfo pipelineCreateInfo
		{
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,					// sType.
			nullptr,															// pNext.
			0,																	// flags.
			static_cast<uint32>(shaderStages.size()),							// stageCount.
			shaderStages.data(),												// pStages.
			&vertexInputCreateInfo,												// pVertexInputState.
			&inputAssemblyCreateInfo,											// pInputAssemblyState.
			nullptr,															// pTessellationState.
			&viewportStateCreateInfo,											// pViewportState.
			&rasterizerCreateInfo,												// pRasterizationState.
			&multisamplingCreateInfo,											// pMultisampleState.
			nullptr,															// pDepthStencilState.
			&colorBlendingCreateInfo,											// pColorBlendState.
			nullptr,															// pDynamicState.
			mPipelineLayout,													// layout.
			renderPass,															// renderPass.
			0,																	// subpass.
			VK_NULL_HANDLE,														// basePipelineHandle.
			-1																	// basePipelineIndex.
		};

		SA_VK_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create graphics pipeline!");
	}

	void VkRenderPipeline::CreateDescriptors(const VkDevice& _device, const PipelineCreateInfos& _pipelineInfos)
	{
		const VkRenderSurface& vkSurface = _pipelineInfos.surface.As<VkRenderSurface>();
		const VkSwapChain& swapChain = vkSurface.GetSwapChain();
		const uint32 imageNum = swapChain.GetImageNum();

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.reserve(_pipelineInfos.textures.size() + 1);

		std::vector<VkDescriptorPoolSize> poolSizes;
		poolSizes.reserve(_pipelineInfos.textures.size() + 1);

		// UBO binding.
		layoutBindings.push_back(VkDescriptorSetLayoutBinding
		{
			0,													// binding.
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,					// descriptorType.
			1,													// descriptorCount.
			VK_SHADER_STAGE_VERTEX_BIT,							// stageFlags.
			nullptr												// pImmutableSamplers.
		});

		poolSizes.push_back(VkDescriptorPoolSize
		{
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,										// type.
			imageNum,																// descriptorCount.
		});


		// Texture binding.
		for (auto it = _pipelineInfos.textures.begin(); it != _pipelineInfos.textures.end(); ++it)
		{
			layoutBindings.push_back(VkDescriptorSetLayoutBinding
			{
				static_cast<uint32>(layoutBindings.size()),			// binding.
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,					// descriptorType.
				1,													// descriptorCount.
				VK_SHADER_STAGE_FRAGMENT_BIT,						// stageFlags.
				nullptr												// pImmutableSamplers.
			});

			poolSizes.push_back(VkDescriptorPoolSize
			{
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,								// type.
				imageNum,																// descriptorCount.
			});
		}


		// Create descriptor set layout.
		const VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,					// sType.
			nullptr,																// pNext.
			0,																		// flags.
			static_cast<uint32>(layoutBindings.size()),								// bindingCount.
			layoutBindings.data()													// pBindings.
		};

		SA_VK_ASSERT(vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutInfo, nullptr, &mDescriptorSetLayout),
			CreationFailed, Rendering, L"Failed to create descriptor set layout!");


		// Create descriptor pool.
		const VkDescriptorPoolCreateInfo descriptorPoolInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,							// sType.
			nullptr,																// pNext.
			0,																		// flags.
			imageNum,																// maxSets.
			static_cast<uint32>(poolSizes.size()),									// poolSizeCount.
			poolSizes.data(),														// pPoolSizes.
		};

		SA_VK_ASSERT(vkCreateDescriptorPool(_device, &descriptorPoolInfo, nullptr, &mDescriptorPool),
			CreationFailed, Rendering, L"Failed to create descriptor pool!");


		// Create Descriptor sets.
		mDescriptorSets.resize(imageNum);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(imageNum, mDescriptorSetLayout);

		const VkDescriptorSetAllocateInfo descriptorSetAllocInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				// sType.
			nullptr,													// pNext.
			mDescriptorPool,											// descriptorPool.
			imageNum,													// descriptorSetCount.
			descriptorSetLayouts.data(),								// pSetLayouts.
		};

		SA_VK_ASSERT(vkAllocateDescriptorSets(_device, &descriptorSetAllocInfo, mDescriptorSets.data()),
			MemoryAllocationFailed, Rendering, L"Failed to allocate descriptor sets!");


		// Init each descriptor set.
		const std::vector<VkBuffer>& uniformBuffers = swapChain.GetUniformBuffers();

		for (uint32 i = 0; i < imageNum; ++i)
		{
			std::vector<VkWriteDescriptorSet> descriptorWrites;
			descriptorWrites.reserve(_pipelineInfos.textures.size() + 1);

			// UBO Binding
			descriptorWrites.push_back(uniformBuffers[i].CreateWriteDescriptorSet(mDescriptorSets[i], static_cast<uint32>(descriptorWrites.size()), sizeof(UniformBufferObject)));


			// Texture binding.
			for (auto it = _pipelineInfos.textures.begin(); it != _pipelineInfos.textures.end(); ++it)
			{
				SA_ASSERT((*it), Nullptr, Rendering, L"Pipeline bind nullptr texture!")

				const VkTexture& vkTexture = (*it)->As<VkTexture>();

				descriptorWrites.push_back(vkTexture.CreateWriteDescriptorSet(mDescriptorSets[i], static_cast<uint32>(descriptorWrites.size())));
			}

			vkUpdateDescriptorSets(_device, static_cast<uint32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

	void VkRenderPipeline::DestroyDescriptors(const VkDevice& _device)
	{
		vkFreeDescriptorSets(_device, mDescriptorPool, static_cast<uint32>(mDescriptorSets.size()), mDescriptorSets.data());
		mDescriptorPool = VK_NULL_HANDLE;

		vkDestroyDescriptorSetLayout(_device, mDescriptorSetLayout, nullptr);
		mDescriptorSetLayout = VK_NULL_HANDLE;

		vkDestroyDescriptorPool(_device, mDescriptorPool, nullptr);
		mDescriptorPool = VK_NULL_HANDLE;
	}

	void VkRenderPipeline::Create(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos)
	{
		Create_Internal(_instance, _pipelineInfos);

		// TODO: CLEAN.
		_pipelineInfos.surface.onResizeEvent.Add(std::function<void(const IRenderInstance&, const IRenderSurface&)>(
			[this, &_pipelineInfos](const IRenderInstance& _instance, const IRenderSurface& _surface)
			{
				(void)_surface;
				ReCreate(_instance, _pipelineInfos);
			}
		));
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

	void VkRenderPipeline::Bind(const IRenderFrame& _frame)
	{
		const VkRenderFrame& vkFrame = _frame.As<VkRenderFrame>();

		vkCmdBindPipeline(vkFrame.graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mHandle);

		vkCmdBindDescriptorSets(vkFrame.graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout,
			0, 1, &mDescriptorSets[vkFrame.frameIndex], 0, nullptr);
	}

	void VkRenderPipeline::ReCreate(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos)
	{
		Destroy(_instance);

		Create_Internal(_instance, _pipelineInfos);
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
