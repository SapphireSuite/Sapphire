// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkShadowCubemapPipeline.hpp>

#include <Rendering/Framework/Primitives/Light/LightViewInfos.hpp>
#include <Rendering/Framework/Primitives/Material/RawMaterial.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>

namespace Sa
{
	VkShadowCubemapPipeline* VkShadowCubemapPipeline::instance = nullptr;


	void VkShadowCubemapPipeline::Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		CreateUniformBuffers(_instance);

		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		ImageExtent extent{ 1200u, 1200u };

		// Depth buffer.
		{
			VkImageBufferCreateInfos depthImageInfos;
			depthImageInfos.format = VK_FORMAT_D32_SFLOAT;
			depthImageInfos.extent = extent;
			depthImageInfos.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
			depthImageInfos.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
			depthImageInfos.layerNum = 6u;
			depthImageInfos.imageViewType = VK_IMAGE_VIEW_TYPE_CUBE;
			depthImageInfos.imageFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

			mDepthCubemap.Create(_instance.As<VkRenderInstance>().GetDevice(), depthImageInfos);
		}


		// CreateTextureSampler. // TODO: Sampler is not link to 1 image: Use 1 for multiple image!
		const VkSamplerCreateInfo samplerCreateInfo
		{
			VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,						// sType.
			nullptr,													// pNext.
			0,															// flags.
			VK_FILTER_LINEAR,											// magFilter.
			VK_FILTER_LINEAR,											// minFilter.
			VK_SAMPLER_MIPMAP_MODE_LINEAR,								// mipmapMode.
			VK_SAMPLER_ADDRESS_MODE_REPEAT,								// addressModeU.
			VK_SAMPLER_ADDRESS_MODE_REPEAT,								// addressModeV.
			VK_SAMPLER_ADDRESS_MODE_REPEAT,								// addressModeW.
			0.0f,														// mipLodBias.
			VK_TRUE,													// anisotropyEnable.
			16.0f,														// maxAnisotropy.
			VK_FALSE,													// compareEnable.
			VK_COMPARE_OP_ALWAYS,										// compareOp.
			0.0f,														// minLod.
			1.0f,														// maxLod.
			VK_BORDER_COLOR_INT_OPAQUE_BLACK,							// borderColor
			VK_FALSE,													// unnormalizedCoordinates.
		};

		SA_VK_ASSERT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &mSampler),
			CreationFailed, Rendering, L"Failed to create texture sampler!");



		// Depth Attachement.
		const VkAttachmentDescription depthAttachment
		{
			0,														// flags.
			VK_FORMAT_D32_SFLOAT,									// format.
			VK_SAMPLE_COUNT_1_BIT,		// samples.
			VK_ATTACHMENT_LOAD_OP_CLEAR,							// loadOp.
			VK_ATTACHMENT_STORE_OP_STORE,							// storeOp.
			VK_ATTACHMENT_LOAD_OP_CLEAR,							// stencilLoadOp.
			VK_ATTACHMENT_STORE_OP_DONT_CARE,						// stencilStoreOp.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL		// finalLayout.
		};


		const VkAttachmentReference depthAttachmentRef
		{
			0u,			// attachment.
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,		// layout.
		};


		const VkSubpassDescription subpass
		{
			0,														// flags.
			VK_PIPELINE_BIND_POINT_GRAPHICS,						// pipelineBindPoint.
			0,														// inputAttachmentCount.
			nullptr,												// pInputAttachments.
			0,														// colorAttachmentCount.
			nullptr,												// pColorAttachments.
			nullptr,												// pResolveAttachments.
			&depthAttachmentRef,									// pDepthStencilAttachment.
			0,														// preserveAttachmentCount.
			nullptr													// pPreserveAttachments.

		};


		VkAttachmentDescription attachements[]{ depthAttachment };

		const VkRenderPassCreateInfo renderPassCreateInfo
		{
			VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,				// sType.
			nullptr,												// pNext.
			VK_RENDER_PASS_CREATE_TRANSFORM_BIT_QCOM,				// flags.
			SizeOf(attachements),									// attachmentCount.
			attachements,									// pAttachments.
			1,														// subpassCount.
			&subpass,												// pSubpasses.
			0,														// dependencyCount.
			nullptr											// pDependencies.
		};

		SA_VK_ASSERT(vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &mRenderPass),
			CreationFailed, Rendering, L"Failed to create render pass!");


		// Framebuffer
		VkImageView depthImageView = mDepthCubemap;

		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.flags = 0;
		framebufferCreateInfo.renderPass = mRenderPass;
		framebufferCreateInfo.attachmentCount = SizeOf(attachements);
		framebufferCreateInfo.pAttachments = &depthImageView;
		framebufferCreateInfo.width = extent.width;
		framebufferCreateInfo.height = extent.height;
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



		// Pipeline.
		//VkRenderPipeline::Create_Internal(_instance, _infos);

		const VkRenderInstance& vkInstance = _instance.As<VkRenderInstance>();


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
			VK_SAMPLE_COUNT_1_BIT,		// rasterizationSamples.
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
			mRenderPass,														// renderPass.
			0,																		// subpass.
			VK_NULL_HANDLE,															// basePipelineHandle.
			-1																		// basePipelineIndex.
		};

		SA_VK_ASSERT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create graphics pipeline!");
	}

	void VkShadowCubemapPipeline::Destroy(const IRenderInstance& _instance)
	{
		VkRenderPipeline::Destroy(_instance);

		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		vkFreeCommandBuffers(device, device.GetGraphicsQueue().GetCommandPool(), 1, &mPrimaryCommandBuffer.Get());

		vkDestroyRenderPass(device, mRenderPass, nullptr);

		mDepthCubemap.Destroy(_instance.As<VkRenderInstance>().GetDevice());

		DestroyUniformBuffers(_instance);
	}


	void VkShadowCubemapPipeline::CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
		std::vector<VkSpecConstantRange>& _specConstRanges, const PipelineCreateInfos& _infos)
	{
		_shaderStages.emplace_back(VkPipelineShaderStageCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
			nullptr,															// pNext.
			0,																	// flags.
			VK_SHADER_STAGE_VERTEX_BIT,											// stage.
			_infos.shaders[0].shader->As<VkShader>(),							// module.
			"main",																// pName.
			nullptr,															// pSpecializationInfo.
		});

		_shaderStages.emplace_back(VkPipelineShaderStageCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
			nullptr,															// pNext.
			0,																	// flags.
			VK_SHADER_STAGE_GEOMETRY_BIT,										// stage.
			_infos.shaders[1].shader->As<VkShader>(),							// module.
			"main",																// pName.
			nullptr,															// pSpecializationInfo.
		});

		_shaderStages.emplace_back(VkPipelineShaderStageCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
			nullptr,															// pNext.
			0,																	// flags.
			VK_SHADER_STAGE_FRAGMENT_BIT,										// stage.
			_infos.shaders[2].shader->As<VkShader>(),							// module.
			"main",																// pName.
			nullptr,															// pSpecializationInfo.
		});
	}

	void VkShadowCubemapPipeline::CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		VkDescriptorSetLayoutBinding layoutBindings[]
		{
			// Light view UBO binding.
			{
				0,																		// binding.
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,										// descriptorType.
				1,																		// descriptorCount.
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,				// stageFlags.
				nullptr																	// pImmutableSamplers.
			},

			// Object UBO binding.
			{
				1,																		// binding.
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,										// descriptorType.
				1,																		// descriptorCount.
				VK_SHADER_STAGE_VERTEX_BIT,												// stageFlags.
				nullptr																	// pImmutableSamplers.
			}
		};


		// Create DescriptorSetLayout.
		const VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,						// sType.
			nullptr,																	// pNext.
			0,																			// flags.
			SizeOf(layoutBindings),														// bindingCount.
			layoutBindings																// pBindings.
		};

		SA_VK_ASSERT(vkCreateDescriptorSetLayout(_instance.GetDevice(), &descriptorSetLayoutInfo, nullptr, &mDescriptorSetLayout),
			CreationFailed, Rendering, L"Failed to create descriptor set layout!");
	}

	void VkShadowCubemapPipeline::CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum)
	{
		VkDescriptorPoolSize poolSizes[]
		{
			{
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,									// type.
				2																	// descriptorCount.
			}
		};


		// Create DescriptorPool.
		const VkDescriptorPoolCreateInfo descriptorPoolInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,							// sType.
			nullptr,																// pNext.
			0,																		// flags.
			_imageNum,																// maxSets.
			SizeOf(poolSizes),														// poolSizeCount.
			poolSizes,																// pPoolSizes.
		};

		SA_VK_ASSERT(vkCreateDescriptorPool(_instance.GetDevice(), &descriptorPoolInfo, nullptr, &mDescriptorPool),
			CreationFailed, Rendering, L"Failed to create descriptor pool!");
	}

	void VkShadowCubemapPipeline::CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum)
	{
		SA_ASSERT(mDescriptorSetLayout != VK_NULL_HANDLE, Nullptr, Rendering, L"Create descriptor sets with null descriptor layout!");
		SA_ASSERT(mDescriptorPool != VK_NULL_HANDLE, Nullptr, Rendering, L"Create descriptor sets with null descriptor pool!");


		// Allocate Descriptor sets.
		mDescriptorSets.resize(1);
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(1, mDescriptorSetLayout);

		const VkDescriptorSetAllocateInfo descriptorSetAllocInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				// sType.
			nullptr,													// pNext.
			mDescriptorPool,											// descriptorPool.
			1,													// descriptorSetCount.
			descriptorSetLayouts.data(),								// pSetLayouts.
		};

		SA_VK_ASSERT(vkAllocateDescriptorSets(_instance.GetDevice(), &descriptorSetAllocInfo, mDescriptorSets.data()),
			MemoryAllocationFailed, Rendering, L"Failed to allocate descriptor sets!");


		// Init each descriptor set.
		VkDescriptorBufferInfo descriptorBuffers[]
		{
			mLightViewUniformBuffer.CreateDescriptorBufferInfo(sizeof(LightViewInfos)),
			mModelUniformBuffer.CreateDescriptorBufferInfo(),
		};

		VkWriteDescriptorSet descriptorWrites[]
		{
			VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[0], 0),
			VkGPUStorageBuffer<Mat4f>::CreateWriteDescriptorSet(mDescriptorSets[0], 1),
		};

		descriptorWrites[0].pBufferInfo = &descriptorBuffers[0];
		descriptorWrites[1].pBufferInfo = &descriptorBuffers[1];

		vkUpdateDescriptorSets(_instance.GetDevice(), SizeOf(descriptorWrites), descriptorWrites, 0, nullptr);
	}


	void VkShadowCubemapPipeline::CreateUniformBuffers(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		// TODO: CLEAN (called in main).
		//mModelUniformBuffer.Create(_instance);

		mLightViewUniformBuffer.Create(device, sizeof(LightViewInfos),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
	
	void VkShadowCubemapPipeline::DestroyUniformBuffers(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		mModelUniformBuffer.Destroy(_instance);
		mLightViewUniformBuffer.Destroy(device);
	}

	void VkShadowCubemapPipeline::InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset)
	{
	}
	void VkShadowCubemapPipeline::UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset)
	{
	}


	void VkShadowCubemapPipeline::Begin()
	{
		vkResetCommandBuffer(mPrimaryCommandBuffer, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

		VkCommandBufferBeginInfo commandBufferBeginInfo{};
		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.pNext = nullptr;
		commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
		commandBufferBeginInfo.pInheritanceInfo = nullptr;

		SA_VK_ASSERT(vkBeginCommandBuffer(mPrimaryCommandBuffer, &commandBufferBeginInfo),
			LibCommandFailed, Rendering, L"Failed to begin command buffer!");

		ImageExtent extent{ 1200u, 800u };

		VkViewport viewport = {};
		viewport.width = extent.width;
		viewport.height = extent.height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;
		vkCmdSetViewport(mPrimaryCommandBuffer, 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.offset = { 0,0 };
		scissor.extent = { extent.width, extent.height };
		vkCmdSetScissor(mPrimaryCommandBuffer, 0, 1, &scissor);

		Next();
	}

	void VkShadowCubemapPipeline::Next()
	{
		ImageExtent extent{ 1200u, 800u };

		VkClearValue clearValue{ 1.f, 0.f };

		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = mRenderPass;
		renderPassBeginInfo.framebuffer = mFramebuffer;
		renderPassBeginInfo.renderArea = VkRect2D{ VkOffset2D{}, extent };
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearValue;

		//vkCmdPipelineBarrier(mPrimaryCommandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_DEPENDENCY_BY_REGION_BIT, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE, 0, VK_NULL_HANDLE);

		vkCmdBeginRenderPass(mPrimaryCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VkShadowCubemapPipeline::End()
	{
		vkCmdEndRenderPass(mPrimaryCommandBuffer);

		SA_VK_ASSERT(vkEndCommandBuffer(mPrimaryCommandBuffer),
			LibCommandFailed, Rendering, L"Failed to end command buffer!");
	}
}