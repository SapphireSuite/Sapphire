// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex.hpp>
#include <Rendering/Framework/Primitives/Material/RawMaterial.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderPass.hpp>
#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>
#include <Rendering/Vulkan/Primitives/Camera/VkCamera.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderPipeline::Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		const VkRenderInstance& vkInstance = _instance.As<VkRenderInstance>();
		const VkDevice& device = vkInstance.GetDevice();
		const VkRenderPass& vkRenderPass = _infos.renderPass->As<VkRenderPass>();

		if(_infos.uniformBufferSize > 0u)
			CreateUniformBuffers(device, _infos, vkRenderPass.GetImageNum());

		CreateDescriptors(vkInstance, _infos);


		std::vector<VkSpecConstantRange> specConstRanges;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		CreateShaderStages(shaderStages, specConstRanges, _infos);


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
		std::vector<VkViewport> viewports; std::vector<VkRect2D> scissors;
		const VkPipelineViewportStateCreateInfo viewportStateCreateInfo = CreateViewportStateCreateInfo(_infos, viewports, scissors);

		VkDynamicState dynamicState[]{ VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		const VkPipelineDynamicStateCreateInfo DynamiCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,					// sType.
			nullptr,																// pNext.
			0,																		// flags.
			SizeOf(dynamicState),													// dynamicStateCount.
			dynamicState															// pDynamicStates.
		};


		// === Create Resterizer ===
		const VkPipelineRasterizationStateCreateInfo rasterizerCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_FALSE,																// depthClampEnable.
			VK_FALSE,																// rasterizerDiscardEnable.
			API_GetPolygonMode(_infos.polygonMode),				// polygonMode.
			API_GetCullingMode(_infos.cullingMode),				// cullMode.
			API_GetFrontFaceMode(_infos.frontFaceMode),			// frontFace.
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

		const VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,				// sType.
			nullptr,																// pNext.
			0,																		// flags.
			VK_TRUE,																// depthTestEnable.
			VK_TRUE,																// depthWriteEnable.
			VK_COMPARE_OP_LESS,														// depthCompareOp.
			VK_FALSE,																// depthBoundsTestEnable.
			VK_FALSE,																// stencilTestEnable.
			{},																		// front.
			{},																		// back.
			0.0f,																	// minDepthBounds.
			1.0f,																	// maxDepthBounds.
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



		const VkGraphicsPipelineCreateInfo pipelineCreateInfo
		{
			VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,					// sType.
			nullptr,															// pNext.
			0,																	// flags.
			SizeOf(shaderStages),												// stageCount.
			shaderStages.data(),												// pStages.
			&vertexInputCreateInfo,												// pVertexInputState.
			&inputAssemblyCreateInfo,											// pInputAssemblyState.
			nullptr,															// pTessellationState.
			&viewportStateCreateInfo,											// pViewportState.
			&rasterizerCreateInfo,												// pRasterizationState.
			&multisamplingCreateInfo,											// pMultisampleState.
			&depthStencilCreateInfo,											// pDepthStencilState.
			&colorBlendingCreateInfo,											// pColorBlendState.
			_infos.bDynamicViewport ? &DynamiCreateInfo : nullptr,				// pDynamicState.
			mPipelineLayout,													// layout.
			vkRenderPass,														// renderPass.
			0,																	// subpass.
			VK_NULL_HANDLE,														// basePipelineHandle.
			-1																	// basePipelineIndex.
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
		static int specs[2] = {0, 2};

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


	void VkRenderPipeline::CreateDescriptorSetLayoutBindings(const PipelineCreateInfos& _infos,
		std::vector<VkDescriptorSetLayoutBinding>& _layoutBindings) const noexcept
	{
		// Static UBO binding.
		_layoutBindings.push_back(VkDescriptorSetLayoutBinding
		{
			0,																	// binding.
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// descriptorType.
			SizeOf(_infos.cameras),										// descriptorCount.
			VK_SHADER_STAGE_VERTEX_BIT,											// stageFlags.
			nullptr																// pImmutableSamplers.
		});


		// Object UBO binding.
		if (_infos.uniformBufferSize > 0u)
		{
			_layoutBindings.push_back(VkDescriptorSetLayoutBinding
			{
				1,																	// binding.
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// descriptorType.
				1,																	// descriptorCount.
				VK_SHADER_STAGE_VERTEX_BIT,											// stageFlags.
				nullptr																// pImmutableSamplers.
			});
		}


		// Texture bindings.
		_layoutBindings.push_back(VkDescriptorSetLayoutBinding
		{
			2,																		// binding.
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,								// descriptorType.
			_infos.textures.GetTextureNum(),										// descriptorCount.
			VK_SHADER_STAGE_FRAGMENT_BIT,											// stageFlags.
			nullptr																	// pImmutableSamplers.
		});


		// Light bindings.
		if (_infos.illumModel != IlluminationModel::None)
		{
			// Material UBO bindings.
			_layoutBindings.push_back(VkDescriptorSetLayoutBinding
			{
				3,																	// binding.
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// descriptorType.
				1,																	// descriptorCount.
				VK_SHADER_STAGE_FRAGMENT_BIT,										// stageFlags.
				nullptr																// pImmutableSamplers.
			});


			// Light buffers binding.
			for (uint32 i = 0u; i < static_cast<uint32>(LightType::Max); ++i)
			{
				_layoutBindings.push_back(VkDescriptorSetLayoutBinding
				{
					4 + i,																// binding.
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,									// descriptorType.
					1,																	// descriptorCount.
					VK_SHADER_STAGE_FRAGMENT_BIT,										// stageFlags.
					nullptr																// pImmutableSamplers.
				});
			}
		}
	}

	void VkRenderPipeline::CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.reserve(5);


		// Populate bindings.
		CreateDescriptorSetLayoutBindings(_infos, layoutBindings);


		// Create DescriptorSetLayout.
		const VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,					// sType.
			nullptr,																// pNext.
			0,																		// flags.
			SizeOf(layoutBindings),													// bindingCount.
			layoutBindings.data()													// pBindings.
		};

		SA_VK_ASSERT(vkCreateDescriptorSetLayout(_instance.GetDevice(), &descriptorSetLayoutInfo, nullptr, &mDescriptorSetLayout),
			CreationFailed, Rendering, L"Failed to create descriptor set layout!");
	}


	void VkRenderPipeline::CreateDescriptorPoolSize(const PipelineCreateInfos& _infos, uint32 _imageNum,
		std::vector<VkDescriptorPoolSize>& _poolSizes) const noexcept
	{
		// Static UBO binding.
		_poolSizes.push_back(VkDescriptorPoolSize
		{
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,										// type.
			_imageNum,																// descriptorCount.
		});


		// Object UBO binding.
		if (_infos.uniformBufferSize > 0u)
		{
			_poolSizes.push_back(VkDescriptorPoolSize
			{
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// type.
				_imageNum,															// descriptorCount.
			});
		}


		// Texture bindings.
		_poolSizes.push_back(VkDescriptorPoolSize
		{
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,								// type.
			1,																		// descriptorCount.
		});


		// Light bindings.
		if (_infos.illumModel != IlluminationModel::None)
		{
			// Material UBO bindigs.
			_poolSizes.push_back(VkDescriptorPoolSize
			{
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// type.
				1,																	// descriptorCount.
			});


			// Light buffers binding.
			_poolSizes.insert(_poolSizes.end(),
				static_cast<uint8>(LightType::Max),
				VkDescriptorPoolSize
				{
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,									// type.
					1,																	// descriptorCount.
				}
			);
		}
	}

	void VkRenderPipeline::CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum)
	{
		std::vector<VkDescriptorPoolSize> poolSizes;
		poolSizes.reserve(5);


		// Populate poolSizes.
		CreateDescriptorPoolSize(_infos, _imageNum, poolSizes);


		// Create DescriptorPool.
		const VkDescriptorPoolCreateInfo descriptorPoolInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,							// sType.
			nullptr,																// pNext.
			0,																		// flags.
			_imageNum,																// maxSets.
			SizeOf(poolSizes),														// poolSizeCount.
			poolSizes.data(),														// pPoolSizes.
		};

		SA_VK_ASSERT(vkCreateDescriptorPool(_instance.GetDevice(), &descriptorPoolInfo, nullptr, &mDescriptorPool),
			CreationFailed, Rendering, L"Failed to create descriptor pool!");
	}


	void VkRenderPipeline::CreateWriteDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _index,
		std::vector<DescriptorInfo>& _descriptorInfos,
		std::vector<VkWriteDescriptorSet>& _descriptorWrites) const noexcept
	{
		// Camera UBO binding.
		for (uint32 i = 0u; i < SizeOf(_infos.cameras); ++i)
		{
			SA_ASSERT(_infos.cameras[i], Nullptr, Rendering, L"Pipeline bind nullptr camera!");

			const VkCamera& camera = _infos.cameras[i]->As<VkCamera>();

			_descriptorInfos.push_back(camera.CreateDescriptorBufferInfo(_index));
			_descriptorWrites.push_back(VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[_index], 0, i));
		}


		// Object UBO binding.
		if (_infos.uniformBufferSize > 0u)
		{
			_descriptorInfos.push_back(mObjectUniformBuffers[_index].CreateDescriptorBufferInfo(_infos.uniformBufferSize));
			_descriptorWrites.push_back(VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[_index], 1));
		}


		// Texture bindings.
		for (uint32 i = 0u; i < MaterialTextures::size; ++i)
		{
			if (!_infos.textures.data[i])
				continue;

			const VkTexture& vkTexture = _infos.textures.data[i]->As<VkTexture>();

			_descriptorInfos.push_back(vkTexture.CreateDescriptorImageInfo());
			_descriptorWrites.push_back(VkTexture::CreateWriteDescriptorSet(mDescriptorSets[_index], 2, i));
		}


		// Light bindings.
		if (_infos.illumModel != IlluminationModel::None)
		{
			// Material UBO bindigs.
			_descriptorInfos.push_back(mMatConstantUniformBuffer.CreateDescriptorBufferInfo(sizeof(MaterialConstants)));
			_descriptorWrites.push_back(VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[_index], 3));


			// Directionnal light buffer bindigs.
			const VkStorageBuffer<DLightInfos>& dLightBuffer = _instance.GetDirectionnalLightBuffer();

			_descriptorInfos.push_back(dLightBuffer.CreateDescriptorBufferInfo());
			_descriptorWrites.push_back(dLightBuffer.CreateWriteDescriptorSet(mDescriptorSets[_index], 4));

			// Point light buffer bindigs.
			const VkStorageBuffer<PLightInfos>& pLightBuffer = _instance.GetPointLightBuffer();

			_descriptorInfos.push_back(pLightBuffer.CreateDescriptorBufferInfo());
			_descriptorWrites.push_back(pLightBuffer.CreateWriteDescriptorSet(mDescriptorSets[_index], 5));



			// Spot light buffer bindigs.
			const VkStorageBuffer<SLightInfos>& sLightBuffer = _instance.GetSpotLightBuffer();

			_descriptorInfos.push_back(sLightBuffer.CreateDescriptorBufferInfo());
			_descriptorWrites.push_back(sLightBuffer.CreateWriteDescriptorSet(mDescriptorSets[_index], 6));
		}
	}

	void VkRenderPipeline::CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum)
	{
		SA_ASSERT(mDescriptorSetLayout != VK_NULL_HANDLE, Nullptr, Rendering, L"Create descriptor sets with null descriptor layout!");
		SA_ASSERT(mDescriptorPool != VK_NULL_HANDLE, Nullptr, Rendering, L"Create descriptor sets with null descriptor pool!");


		// Allocate Descriptor sets.
		mDescriptorSets.resize(_imageNum);
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(_imageNum, mDescriptorSetLayout);

		const VkDescriptorSetAllocateInfo descriptorSetAllocInfo
		{
			VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,				// sType.
			nullptr,													// pNext.
			mDescriptorPool,											// descriptorPool.
			_imageNum,													// descriptorSetCount.
			descriptorSetLayouts.data(),								// pSetLayouts.
		};

		SA_VK_ASSERT(vkAllocateDescriptorSets(_instance.GetDevice(), &descriptorSetAllocInfo, mDescriptorSets.data()),
			MemoryAllocationFailed, Rendering, L"Failed to allocate descriptor sets!");


		// Init each descriptor set.
		std::vector<DescriptorInfo> descriptorInfos;
		descriptorInfos.reserve(5);

		std::vector<VkWriteDescriptorSet> descriptorWrites;
		descriptorWrites.reserve(5);

		for (uint32 i = 0; i < _imageNum; ++i)
		{
			descriptorInfos.clear();
			descriptorWrites.clear();

			CreateWriteDescriptorSets(_instance, _infos, i, descriptorInfos, descriptorWrites);

			// Init info ptr after any vector resize.
			for (uint32 j = 0u; j < descriptorWrites.size(); ++j)
			{
				if (descriptorInfos[j].bIsImage)
					descriptorWrites[j].pImageInfo = &descriptorInfos[j].image;
				else
					descriptorWrites[j].pBufferInfo = &descriptorInfos[j].buffer;
			}

			vkUpdateDescriptorSets(_instance.GetDevice(), SizeOf(descriptorWrites), descriptorWrites.data(), 0, nullptr);
		}
	}


	VkPipelineViewportStateCreateInfo VkRenderPipeline::CreateViewportStateCreateInfo(const PipelineCreateInfos& _infos,
		std::vector<VkViewport>& _viewports, std::vector<VkRect2D>& _scissors)
	{
		uint32 num = _infos.cameras.size();

		if (_infos.bDynamicViewport)
		{
			return VkPipelineViewportStateCreateInfo
			{
				VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,					// sType.
				nullptr,																// pNext.
				0,																		// flags.
				num,																	// viewportCount.
				nullptr,																// pViewports.
				num,																	// scissorCount.
				nullptr																	// pScissors.
			};
		}


		// Static viewport and scissors.
		_viewports.reserve(num);
		_scissors.reserve(num);

		for (auto it = _infos.cameras.begin(); it != _infos.cameras.end(); ++it)
		{
			_viewports.push_back((*it)->GetVkViewport());
			_scissors.push_back((*it)->GetVkScissor());
		}

		return VkPipelineViewportStateCreateInfo
		{
			VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,					// sType.
			nullptr,																// pNext.
			0,																		// flags.
			num,																	// viewportCount.
			_viewports.data(),														// pViewports.
			num,																	// scissorCount.
			_scissors.data()														// pScissors.
		};
	}


	void VkRenderPipeline::CreateDescriptors(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		CreateDescriptorSetLayout(_instance, _infos);

		const uint32 imageNum = _infos.renderPass->As<VkRenderPass>().GetImageNum();

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

	void VkRenderPipeline::CreateUniformBuffers(const VkDevice& _device, const PipelineCreateInfos& _infos, uint32 _imageNum)
	{
		mObjectUniformBuffers.resize(_imageNum);

		for (uint32 i = 0; i < _imageNum; i++)
		{
			mObjectUniformBuffers[i].Create(_device, _infos.uniformBufferSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}

		mMatConstantUniformBuffer.Create(_device, sizeof(MaterialConstants),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_infos.matConstants);
	}
	void VkRenderPipeline::DestroyUniformBuffers(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mObjectUniformBuffers.size(); i++)
			mObjectUniformBuffers[i].Destroy(_device);
		
		mObjectUniformBuffers.clear();

		mMatConstantUniformBuffer.Destroy(_device);
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

		if (_infos.illumModel != IlluminationModel::None)
		{
			// TODO: ADD Pipeline recreation event.
		}
	}

	void VkRenderPipeline::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		DestroyDescriptors(device);

		vkDestroyPipeline(device, mHandle, nullptr);
		mHandle = VK_NULL_HANDLE;

		vkDestroyPipelineLayout(device, mPipelineLayout, nullptr);
		mPipelineLayout = VK_NULL_HANDLE;

		DestroyUniformBuffers(device);
	}

	void VkRenderPipeline::Bind(const IRenderFrame& _frame) const
	{
		const VkRenderFrame& vkFrame = _frame.As<VkRenderFrame>();

		vkCmdBindPipeline(vkFrame.graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mHandle);

		vkCmdBindDescriptorSets(vkFrame.graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout,
			0, 1, &mDescriptorSets[vkFrame.index], 0, nullptr);
	}

	void VkRenderPipeline::ReCreate(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		Destroy(_instance);

		Create_Internal(_instance, _infos);
	}

	void VkRenderPipeline::InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		for (auto it = mObjectUniformBuffers.begin(); it != mObjectUniformBuffers.end(); ++it)
			it->UpdateData(device, _data, _dataSize, _offset);
	}
	void VkRenderPipeline::UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		uint32 frameIndex = _frame.As<VkRenderFrame>().index;
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		SA_ASSERT(frameIndex >= 0u && frameIndex < SizeOf(mObjectUniformBuffers), OutOfRange, Rendering, frameIndex, 0u, SizeOf(mObjectUniformBuffers));

		mObjectUniformBuffers[frameIndex].UpdateData(device, _data, _dataSize, _offset);
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
