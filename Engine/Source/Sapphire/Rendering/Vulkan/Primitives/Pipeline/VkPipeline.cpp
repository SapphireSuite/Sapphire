// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkPipeline.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

namespace Sa::Vk
{
	// TODO: REMOVE LATER.
	constexpr uint32 imageNum = 3u;


	struct DescriptorInfo
	{
		union
		{
			VkDescriptorBufferInfo buffer;
			VkDescriptorImageInfo image;
		};

		bool bIsImage = false;

		DescriptorInfo() = default;
		DescriptorInfo(VkDescriptorBufferInfo _buffer) noexcept : buffer{ _buffer }, bIsImage{ false } {}
		DescriptorInfo(VkDescriptorImageInfo _image) noexcept : image{ _image }, bIsImage{ true } {}
	};

	struct RenderPassAttachmentInfos
	{
		VkPipelineMultisampleStateCreateInfo multisamplingInfos{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};

		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
		VkPipelineColorBlendStateCreateInfo colorBlendingInfo{};
	};

	void Pipeline::Create(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		CreateDescriptorSetLayout(device, _infos);
		CreateDescriptorPoolSize(device, _infos);
		CreateDescriptorSets(device, _infos);

		CreatePipelineLayout(device);
		CreatePipelineHandle(device, _infos);
	}
	
	void Pipeline::Destroy(const IRenderInstance& _instance)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		DestroyPipelineHandle(device);
		DestroyPipelineLayout(device);

		DestroyDescriptorSetLayout(device);
		DestroyDescriptorPoolSize(device);
		DestroyDescriptorSets(device);
	}


	void Pipeline::CreateDescriptorSetLayout(const Device& _device, const PipelineCreateInfos& _infos)
	{
		// Fill descriptor set layout bindings.
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.reserve(_infos.bindings.size());

		for (auto it = _infos.bindings.begin(); it != _infos.bindings.end(); ++it)
		{
			VkDescriptorSetLayoutBinding binding{};
			binding.binding = it->binding;
			binding.descriptorType = API_GetDescriptorType(it->type);
			binding.descriptorCount = SizeOf(it->buffers);
			binding.stageFlags = API_GetShaderStageFlags(it->stages);
			binding.pImmutableSamplers = nullptr;

			layoutBindings.push_back(binding);
		}


		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.pNext = nullptr;
		descriptorSetLayoutInfo.flags = 0u;
		descriptorSetLayoutInfo.bindingCount = SizeOf(layoutBindings);
		descriptorSetLayoutInfo.pBindings = layoutBindings.data();

		SA_VK_ASSERT(vkCreateDescriptorSetLayout(_device, &descriptorSetLayoutInfo, nullptr, &mDescriptorSetLayout),
			CreationFailed, Rendering, L"Failed to create descriptor set layout!");
	}

	void Pipeline::DestroyDescriptorSetLayout(const Device& _device)
	{
		vkDestroyDescriptorSetLayout(_device, mDescriptorSetLayout, nullptr);
		mDescriptorSetLayout = VK_NULL_HANDLE;
	}


	void Pipeline::CreateDescriptorPoolSize(const Device& _device, const PipelineCreateInfos& _infos)
	{
		std::vector<VkDescriptorPoolSize> poolSizes;
		poolSizes.resize(4u);

		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[3].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

		for (auto it = _infos.bindings.begin(); it != _infos.bindings.end(); ++it)
		{
			VkDescriptorType type = API_GetDescriptorType(it->type);

			if (type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
				++poolSizes[0].descriptorCount;
			else if (type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
				++poolSizes[1].descriptorCount;
			else if (type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
				++poolSizes[2].descriptorCount;
			else if (type == VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
				++poolSizes[3].descriptorCount;
		}

		// Remove empty descriptor pool.
		for (uint32 i = 0; i < SizeOf(poolSizes); ++i)
		{
			if (poolSizes[i].descriptorCount == 0)
			{
				poolSizes.erase(poolSizes.begin() + i);
				--i;
			}
		}

		VkDescriptorPoolCreateInfo descriptorPoolInfo{};
		descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolInfo.pNext = nullptr;
		descriptorPoolInfo.flags = 0u;
		descriptorPoolInfo.maxSets = imageNum;
		descriptorPoolInfo.poolSizeCount = SizeOf(poolSizes);
		descriptorPoolInfo.pPoolSizes = poolSizes.data();

		SA_VK_ASSERT(vkCreateDescriptorPool(_device, &descriptorPoolInfo, nullptr, &mDescriptorPool),
			CreationFailed, Rendering, L"Failed to create descriptor pool!");
	}

	void Pipeline::DestroyDescriptorPoolSize(const Device& _device)
	{
		vkDestroyDescriptorPool(_device, mDescriptorPool, nullptr);
		mDescriptorPool = VK_NULL_HANDLE;
	}


	void Pipeline::CreateDescriptorSets(const Device& _device, const PipelineCreateInfos& _infos)
	{
		mDescriptorSets.resize(imageNum);
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(imageNum, mDescriptorSetLayout);

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
		descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocInfo.pNext = nullptr;
		descriptorSetAllocInfo.descriptorPool = mDescriptorPool;
		descriptorSetAllocInfo.descriptorSetCount = imageNum;
		descriptorSetAllocInfo.pSetLayouts = descriptorSetLayouts.data();

		SA_VK_ASSERT(vkAllocateDescriptorSets(_device, &descriptorSetAllocInfo, mDescriptorSets.data()),
			MemoryAllocationFailed, Rendering, L"Failed to allocate descriptor sets!");


		UpdateDescriptorSets(_device, _infos.bindings);
	}
	
	void Pipeline::UpdateDescriptorSets(const Device& _device, PipelineDataBindingInfos& _bindInfos)
	{
		std::vector<VkDescriptorBufferInfo> descBufferInfos;
		descBufferInfos.reserve(SizeOf(mDescriptorSets) * SizeOf(_bindInfos.buffers));

		std::vector<VkDescriptorImageInfo> descImageInfos;
		descImageInfos.reserve(SizeOf(mDescriptorSets) * SizeOf(_bindInfos.buffers));

		std::vector<VkWriteDescriptorSet> descWrites;
		descWrites.reserve(SizeOf(mDescriptorSets) * SizeOf(_bindInfos.buffers));


		for (uint32 i = 0; i < SizeOf(mDescriptorSets); ++i)
		{
			for (uint32 j = 0u; j < SizeOf(_bindInfos.buffers); ++j)
			{
				VkWriteDescriptorSet& writeDesc = descWrites.emplace_back();
				writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeDesc.pNext = nullptr;
				writeDesc.dstSet = mDescriptorSets[i];
				writeDesc.dstBinding = _bindInfos.binding;
				writeDesc.dstArrayElement = j;
				writeDesc.descriptorCount = 1u;

				if (_bindInfos.type == ShaderBindingType::UniformBuffer)
				{
					writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

					VkDescriptorBufferInfo& desc = descBufferInfos.emplace_back(VkDescriptorBufferInfo{ _bindInfos.buffers[j]->As<Buffer>().CreateDescriptorBufferInfo(_bindInfos.bufferDataSize) });
					writeDesc.pBufferInfo = &desc;
				}
				else if (_bindInfos.type == ShaderBindingType::StorageBuffer)
				{
					writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

					// TODO: IMPLEMENT.
				}
				else if (_bindInfos.type == ShaderBindingType::ImageSampler2D || _bindInfos.type == ShaderBindingType::ImageSamplerCube)
				{
					writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

					VkDescriptorImageInfo& desc = descImageInfos.emplace_back(VkDescriptorImageInfo{ _bindInfos.buffers[j]->As<Texture>().CreateDescriptorImageInfo() });
					writeDesc.pImageInfo = &desc;
				}
				else if (_bindInfos.type != ShaderBindingType::InputAttachment)
					SA_LOG("ShaderBindingType for descriptor write not supported.", Error, Rendering);
			}
		}

		vkUpdateDescriptorSets(_device, SizeOf(descWrites), descWrites.data(), 0, nullptr);
	}

	void Pipeline::UpdateDescriptorSets(const Device& _device, const std::vector<PipelineBindingInfos>& _bindings)
	{
		std::vector<DescriptorInfo> descInfos;
		descInfos.reserve(10u);

		std::vector<VkWriteDescriptorSet> descWrites;
		descWrites.reserve(10u);

		for (uint32 i = 0; i < SizeOf(mDescriptorSets); ++i)
		{
			for (auto it = _bindings.begin(); it != _bindings.end(); ++it)
			{
				for (uint32 j = 0u; j < SizeOf(it->buffers); ++j)
				{
					VkWriteDescriptorSet& writeDesc = descWrites.emplace_back();
					writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					writeDesc.pNext = nullptr;
					writeDesc.dstSet = mDescriptorSets[i];
					writeDesc.dstBinding = it->binding;
					writeDesc.dstArrayElement = j;
					writeDesc.descriptorCount = 1u;

					if (it->type == ShaderBindingType::UniformBuffer)
					{
						writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

						descInfos.emplace_back(DescriptorInfo{ it->buffers[j]->As<Buffer>().CreateDescriptorBufferInfo(it->bufferDataSize) });
					}
					else if (it->type == ShaderBindingType::StorageBuffer)
					{
						writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

						// TODO: IMPLEMENT.
					}
					else if (it->type == ShaderBindingType::ImageSampler2D || it->type == ShaderBindingType::ImageSamplerCube)
					{
						writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

						descInfos.emplace_back(DescriptorInfo{ it->buffers[j]->As<Texture>().CreateDescriptorImageInfo() });
					}
					else if (it->type != ShaderBindingType::InputAttachment)
						SA_LOG("ShaderBindingType for descriptor write not supported.", Error, Rendering);
				}
			}
		}

		// Init info ptr after any vector resize.
		for (uint32 j = 0u; j < descWrites.size(); ++j)
		{
			if (descInfos[j].bIsImage)
				descWrites[j].pImageInfo = &descInfos[j].image;
			else
				descWrites[j].pBufferInfo = &descInfos[j].buffer;
		}

		vkUpdateDescriptorSets(_device, SizeOf(descWrites), descWrites.data(), 0, nullptr);
	}

	void Pipeline::DestroyDescriptorSets(const Device& _device)
	{
		// Not needed when vkDestroyDescriptorPool() is called. Otherwise, requiere VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT flag.
		//vkFreeDescriptorSets(_device, mDescriptorPool, static_cast<uint32>(mDescriptorSets.size()), mDescriptorSets.data());
		mDescriptorSets.clear();
	}


	void Pipeline::CreatePipelineLayout(const Device& _device)
	{
		VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
		pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutCreateInfo.pNext = nullptr;
		pipelineLayoutCreateInfo.flags = 0u;
		pipelineLayoutCreateInfo.setLayoutCount = 1u;
		pipelineLayoutCreateInfo.pSetLayouts = &mDescriptorSetLayout;
		pipelineLayoutCreateInfo.pushConstantRangeCount = 0u;
		pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;

		SA_VK_ASSERT(vkCreatePipelineLayout(_device, &pipelineLayoutCreateInfo, nullptr, &mPipelineLayout),
			CreationFailed, Rendering, L"Failed to create pipeline layout!");
	}
	
	void Pipeline::DestroyPipelineLayout(const Device& _device)
	{
		vkDestroyPipelineLayout(_device, mPipelineLayout, nullptr);
		mPipelineLayout = VK_NULL_HANDLE;
	}


	void Pipeline::CreatePipelineHandle(const Device& _device, const PipelineCreateInfos& _infos)
	{
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		FillShaderStages(shaderStages, _infos.shaders);


		// Vertex input infos.
		std::unique_ptr<VkVertexInputBindingDescription> bindingDesc;
		std::unique_ptr<VkVertexInputAttributeDescription[]> attribDescs;
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		FillVertexBindings(vertexInputInfo, bindingDesc, attribDescs, _infos.vertexBindingLayout);


		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.pNext = nullptr;
		inputAssemblyInfo.flags = 0;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;


		// Viewport & Scissor.
		// TODO: CLEAN.
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = 1200.0f;
		viewport.height = 800.0f;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = VkOffset2D{ 0, 0 };
		scissor.extent = VkExtent2D{ 1200, 800 };

		VkPipelineViewportStateCreateInfo viewportStateInfo{};
		viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportStateInfo.pNext = nullptr;
		viewportStateInfo.flags = 0u;
		viewportStateInfo.viewportCount = 1u;
		viewportStateInfo.pViewports = &viewport;
		viewportStateInfo.scissorCount = 1u;
		viewportStateInfo.pScissors = &scissor;

		const VkDynamicState dynamicState[]{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo viewportDynamicInfo{};
		viewportDynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		viewportDynamicInfo.pNext = nullptr;
		viewportDynamicInfo.flags = 0u;
		viewportDynamicInfo.dynamicStateCount = SizeOf(dynamicState);
		viewportDynamicInfo.pDynamicStates = dynamicState;


		// Rasterization
		VkPipelineRasterizationStateCreateInfo rasterizerInfo{};
		FillRasterization(rasterizerInfo, _infos.modes);


		// Multisampling.
		RenderPassAttachmentInfos renderPassAttInfos{};
		FillRenderPassAttachments(renderPassAttInfos, _infos.renderPassDesc);


		// Create handle.
		VkGraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineCreateInfo.pNext = nullptr;
		pipelineCreateInfo.flags = 0u;
		pipelineCreateInfo.stageCount = SizeOf(shaderStages);
		pipelineCreateInfo.pStages = shaderStages.data();
		pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineCreateInfo.pTessellationState = nullptr;
		pipelineCreateInfo.pViewportState = &viewportStateInfo;
		pipelineCreateInfo.pRasterizationState = &rasterizerInfo;
		pipelineCreateInfo.pMultisampleState = &renderPassAttInfos.multisamplingInfos;
		pipelineCreateInfo.pDepthStencilState = &renderPassAttInfos.depthStencilInfo;
		pipelineCreateInfo.pColorBlendState = &renderPassAttInfos.colorBlendingInfo;
		pipelineCreateInfo.pDynamicState = nullptr;
		pipelineCreateInfo.layout = mPipelineLayout;
		pipelineCreateInfo.renderPass = _infos.renderPass.As<RenderPass>();
		pipelineCreateInfo.subpass = SizeOf(_infos.renderPassDesc.subPassDescriptors) - 1; // Additionnal subpass.
		pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
		pipelineCreateInfo.basePipelineIndex = -1;

		SA_VK_ASSERT(vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &mHandle),
			CreationFailed, Rendering, L"Failed to create graphics pipeline!");
	}
	
	void Pipeline::DestroyPipelineHandle(const Device& _device)
	{
		vkDestroyPipeline(_device, mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;
	}


	void Pipeline::FillShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _stages, const std::vector<PipelineShaderInfos>& _shaders)
	{
		_stages.reserve(_shaders.size());

		for (auto it = _shaders.begin(); it != _shaders.end(); ++it)
		{
			VkPipelineShaderStageCreateInfo stage;
			stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stage.pNext = nullptr;
			stage.flags = 0u;
			stage.stage = API_GetShaderStage(it->stage);
			stage.module = it->shader->As<Shader>();
			stage.pName = "main";
			stage.pSpecializationInfo = nullptr;

			_stages.push_back(stage);
		}
	}

	void Pipeline::FillVertexBindings(VkPipelineVertexInputStateCreateInfo& _vertexInputInfo, std::unique_ptr<VkVertexInputBindingDescription>& _bindingDesc,
		std::unique_ptr<VkVertexInputAttributeDescription[]>& _attribDescs, const VertexBindingLayout& _vertexBindingLayout) noexcept
	{
		_bindingDesc = _vertexBindingLayout.GetBindingDescription();
		_attribDescs = _vertexBindingLayout.GetAttributeDescriptions();

		_vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		_vertexInputInfo.pNext = nullptr;
		_vertexInputInfo.flags = 0u;
		_vertexInputInfo.vertexBindingDescriptionCount = _vertexBindingLayout.GetBindingDescriptionNum();
		_vertexInputInfo.pVertexBindingDescriptions = _bindingDesc.get();
		_vertexInputInfo.vertexAttributeDescriptionCount = _vertexBindingLayout.GetAttributeDescriptionNum();
		_vertexInputInfo.pVertexAttributeDescriptions = _attribDescs.get();
	}

	void Pipeline::FillRasterization(VkPipelineRasterizationStateCreateInfo& _rasterizerInfo, const PipelineRenderModes& _modes) noexcept
	{
		_rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		_rasterizerInfo.pNext = nullptr;
		_rasterizerInfo.flags = 0u;
		_rasterizerInfo.depthClampEnable = VK_FALSE;
		_rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
		_rasterizerInfo.polygonMode = API_GetPolygonMode(_modes.polygon);
		_rasterizerInfo.cullMode = API_GetCullingMode(_modes.culling);
		_rasterizerInfo.frontFace = API_GetFrontFaceMode(_modes.frontFace);
		_rasterizerInfo.depthBiasEnable = VK_FALSE;
		_rasterizerInfo.depthBiasConstantFactor = 0.0f;
		_rasterizerInfo.depthBiasClamp = 0.0f;
		_rasterizerInfo.depthBiasSlopeFactor = 0.0f;
		_rasterizerInfo.lineWidth = 1.0f;
	}

	void Pipeline::FillRenderPassAttachments(RenderPassAttachmentInfos& _renderPassAttInfos, const RenderPassDescriptor& _renderPassDesc) noexcept
	{
		// MultiSampling.
		_renderPassAttInfos.multisamplingInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		_renderPassAttInfos.multisamplingInfos.pNext = nullptr;
		_renderPassAttInfos.multisamplingInfos.flags = 0u;
		_renderPassAttInfos.multisamplingInfos.rasterizationSamples = static_cast<VkSampleCountFlagBits>(_renderPassDesc.sampling);
		_renderPassAttInfos.multisamplingInfos.sampleShadingEnable = VK_TRUE;
		_renderPassAttInfos.multisamplingInfos.minSampleShading = 0.2f;
		_renderPassAttInfos.multisamplingInfos.pSampleMask = nullptr;
		_renderPassAttInfos.multisamplingInfos.alphaToCoverageEnable = VK_FALSE;
		_renderPassAttInfos.multisamplingInfos.alphaToOneEnable = VK_FALSE;

		// Depth stencil.
		_renderPassAttInfos.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		_renderPassAttInfos.depthStencilInfo.pNext = nullptr;
		_renderPassAttInfos.depthStencilInfo.flags = 0u;
		_renderPassAttInfos.depthStencilInfo.depthTestEnable = VK_TRUE;
		_renderPassAttInfos.depthStencilInfo.depthWriteEnable = VK_TRUE;
		_renderPassAttInfos.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		_renderPassAttInfos.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		_renderPassAttInfos.depthStencilInfo.stencilTestEnable = VK_FALSE;
		_renderPassAttInfos.depthStencilInfo.front = {};
		_renderPassAttInfos.depthStencilInfo.back = {};
		_renderPassAttInfos.depthStencilInfo.minDepthBounds = 0.0f;
		_renderPassAttInfos.depthStencilInfo.maxDepthBounds = 1.0f;


		// Color attachments.
		_renderPassAttInfos.colorBlendAttachments.reserve(10u);
		for (auto subIt = _renderPassDesc.subPassDescriptors.begin(); subIt != _renderPassDesc.subPassDescriptors.end(); ++subIt)
		{
			for (auto attIt = subIt->attachmentDescriptors.begin(); attIt != subIt->attachmentDescriptors.end(); ++attIt)
			{
				VkPipelineColorBlendAttachmentState& colorBlendAttachment = _renderPassAttInfos.colorBlendAttachments.emplace_back();
				colorBlendAttachment.blendEnable = VK_TRUE;
				colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
				colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
				colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
				colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
				colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
				colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			}
		}

		// Color blending.
		_renderPassAttInfos.colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		_renderPassAttInfos.colorBlendingInfo.pNext = nullptr;
		_renderPassAttInfos.colorBlendingInfo.flags = 0u;
		_renderPassAttInfos.colorBlendingInfo.logicOpEnable = VK_FALSE;
		_renderPassAttInfos.colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY;
		_renderPassAttInfos.colorBlendingInfo.attachmentCount = SizeOf(_renderPassAttInfos.colorBlendAttachments);
		_renderPassAttInfos.colorBlendingInfo.pAttachments = _renderPassAttInfos.colorBlendAttachments.data();
		_renderPassAttInfos.colorBlendingInfo.blendConstants[0] = 0.0f;
		_renderPassAttInfos.colorBlendingInfo.blendConstants[1] = 0.0f;
		_renderPassAttInfos.colorBlendingInfo.blendConstants[2] = 0.0f;
		_renderPassAttInfos.colorBlendingInfo.blendConstants[3] = 0.0f;
	}



	void Pipeline::Bind(const FrameInfos& _frameInfos)
	{
		Vk::CommandBuffer& commandBuffer = _frameInfos.frameBuffer.As<Vk::FrameBuffer>().commandBuffer;

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mHandle);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout,
			0, 1, &mDescriptorSets[_frameInfos.frameIndex], 0, nullptr);
	}
}
