// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkPipeline.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>

namespace Sa::Vk
{
	struct RenderPassAttachmentInfos
	{
		VkPipelineMultisampleStateCreateInfo multisamplingInfos{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};

		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
		VkPipelineColorBlendStateCreateInfo colorBlendingInfo{};
	};


	VkPipelineLayout Pipeline::GetLayout() const noexcept
	{
		return mPipelineLayout;
	}

	VkDescriptorSetLayout Pipeline::GetDescriptorSetLayout() const noexcept
	{
		return mDescriptorSetLayout;
	}


	void Pipeline::Create(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		CreateDescriptorSetLayout(device, _infos);

		CreatePipelineLayout(device);
		CreatePipelineHandle(device, _infos);
	}
	
	void Pipeline::Destroy(const IRenderInstance& _instance)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		DestroyPipelineHandle(device);
		DestroyPipelineLayout(device);

		DestroyDescriptorSetLayout(device);
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
			binding.descriptorCount = it->num;
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
	}
}
