// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkRenderPipeline.hpp>

#include <Core/Types/Variadics/SizeOf.hpp>

#include <Rendering/Framework/Primitives/Mesh/Vertex.hpp>
#include <Rendering/Framework/Primitives/Shader/ShaderType.hpp>
#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>
#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/System/VkRenderSurface.hpp>
#include <Rendering/Vulkan/Primitives/Shader/VkShader.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkRenderPipeline::Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos)
	{
		const VkRenderInstance& vkInstance = _instance.As<VkRenderInstance>();
		const VkDevice& device = vkInstance.GetDevice();
		const VkRenderSurface& vkSurface = _pipelineInfos.surface.As<VkRenderSurface>();

		if(_pipelineInfos.uniformBufferSize > 0u)
			CreateUniformBuffers(device, _pipelineInfos, vkSurface.GetSwapChain().GetImageNum());

		CreateDescriptors(vkInstance, _pipelineInfos);


		std::vector<ShaderStageCreateInfos> stagesInfos;
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
		CreateShaderStages(shaderStages, stagesInfos, _pipelineInfos);


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

	void VkRenderPipeline::CreateShaderStages(std::vector<VkPipelineShaderStageCreateInfo>& _shaderStages,
		std::vector<ShaderStageCreateInfos>& _stagesInfos, const PipelineCreateInfos& _pipelineInfos)
	{
		_stagesInfos.reserve(5u);
		_shaderStages.reserve(5u);

		// Create vertex shader stage.
		if (_pipelineInfos.vertexShader)
		{
			_shaderStages.push_back(VkPipelineShaderStageCreateInfo
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
			ShaderStageCreateInfos& stageInfos = _stagesInfos.emplace_back();

			stageInfos.specEntries =
			{
				VkSpecializationMapEntry

				// Alpha model.
				{
					1,									// constantID.
					0,									// offset.
					sizeof(int)							// size.
				},
				
				// Illumination Model.
				{
					2,									// constantID.
					sizeof(int),						// offset.
					sizeof(int)							// size.
				}
			};

			stageInfos.specInfos = VkSpecializationInfo
			{
				SizeOf(stageInfos.specEntries),			// mapEntryCount.
				stageInfos.specEntries.data(),			// pMapEntries.
				2 * sizeof(int),						// dataSize.
				&_pipelineInfos.alphaModel				// pData.
			};

			_shaderStages.push_back(VkPipelineShaderStageCreateInfo
			{
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,				// sType.
				nullptr,															// pNext.
				0,																	// flags.
				API_GetShaderType(ShaderType::Fragment),							// stage.
				_pipelineInfos.fragementShader->As<VkShader>(),						// module.
				"main",																// pName.
				&stageInfos.specInfos,												// pSpecializationInfo.
			});
		}
	}


	void VkRenderPipeline::CreateDescriptorSetLayoutBindings(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos,
		std::vector<VkDescriptorSetLayoutBinding>& _layoutBindings) const noexcept
	{
		// Static UBO binding.
		_layoutBindings.push_back(VkDescriptorSetLayoutBinding
		{
			0,																	// binding.
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// descriptorType.
			1,																	// descriptorCount.
			VK_SHADER_STAGE_VERTEX_BIT,											// stageFlags.
			nullptr																// pImmutableSamplers.
		});


		// Object UBO binding.
		if (_pipelineInfos.uniformBufferSize > 0u)
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
			SizeOf(_pipelineInfos.textures),										// descriptorCount.
			VK_SHADER_STAGE_FRAGMENT_BIT,											// stageFlags.
			nullptr																	// pImmutableSamplers.
		});


		// Light bindings.
		if (_pipelineInfos.illumModel != IlluminationModel::None)
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

	void VkRenderPipeline::CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos)
	{
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
		layoutBindings.reserve(5);


		// Populate bindings.
		CreateDescriptorSetLayoutBindings(_instance, _pipelineInfos, layoutBindings);


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


	void VkRenderPipeline::CreateDescriptorPoolSize(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum,
		std::vector<VkDescriptorPoolSize>& _poolSizes) const noexcept
	{
		// Static UBO binding.
		_poolSizes.push_back(VkDescriptorPoolSize
		{
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,										// type.
			_imageNum,																// descriptorCount.
		});


		// Object UBO binding.
		if (_pipelineInfos.uniformBufferSize > 0u)
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
		if (_pipelineInfos.illumModel != IlluminationModel::None)
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

	void VkRenderPipeline::CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum)
	{
		std::vector<VkDescriptorPoolSize> poolSizes;
		poolSizes.reserve(5);


		// Populate poolSizes.
		CreateDescriptorPoolSize(_instance, _pipelineInfos, _imageNum, poolSizes);


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


	void VkRenderPipeline::CreateWriteDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos, uint32 _index,
		std::vector<DescriptorInfo>& _descriptorInfos,
		std::vector<VkWriteDescriptorSet>& _descriptorWrites) const noexcept
	{
		const std::vector<VkUniformBuffer>& uniformBuffers = _pipelineInfos.surface.As<VkRenderSurface>().GetSwapChain().GetUniformBuffers();

		// Static UBO binding.
		_descriptorInfos.push_back(uniformBuffers[_index].CreateDescriptorBufferInfo(sizeof(StaticUniformBuffer)));
		_descriptorWrites.push_back(VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[_index], 0));


		// Object UBO binding.
		if (_pipelineInfos.uniformBufferSize > 0u)
		{
			_descriptorInfos.push_back(mObjectUniformBuffers[_index].CreateDescriptorBufferInfo(_pipelineInfos.uniformBufferSize));
			_descriptorWrites.push_back(VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[_index], 1));
		}


		// Texture bindings.
		for (uint32 i = 0u; i < SizeOf(_pipelineInfos.textures); ++i)
		{
			SA_ASSERT(_pipelineInfos.textures[i], Nullptr, Rendering, L"Pipeline bind nullptr texture!");

			const VkTexture& vkTexture = _pipelineInfos.textures[i]->As<VkTexture>();

			_descriptorInfos.push_back(vkTexture.CreateDescriptorImageInfo());
			_descriptorWrites.push_back(VkTexture::CreateWriteDescriptorSet(mDescriptorSets[_index], 2, i));
		}


		// Light bindings.
		if (_pipelineInfos.illumModel != IlluminationModel::None)
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

	void VkRenderPipeline::CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum)
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

			CreateWriteDescriptorSets(_instance, _pipelineInfos, i, descriptorInfos, descriptorWrites);

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


	void VkRenderPipeline::CreateDescriptors(const VkRenderInstance& _instance, const PipelineCreateInfos& _pipelineInfos)
	{
		CreateDescriptorSetLayout(_instance, _pipelineInfos);

		const uint32 imageNum = _pipelineInfos.surface.As<VkRenderSurface>().GetSwapChain().GetImageNum();

		CreateDescriptorPool(_instance, _pipelineInfos, imageNum);
		CreateDescriptorSets(_instance, _pipelineInfos, imageNum);
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

	void VkRenderPipeline::CreateUniformBuffers(const VkDevice& _device, const PipelineCreateInfos& _pipelineInfos, uint32 _imageNum)
	{
		mObjectUniformBuffers.resize(_imageNum);

		for (uint32 i = 0; i < _imageNum; i++)
		{
			mObjectUniformBuffers[i].Create(_device, _pipelineInfos.uniformBufferSize,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		}

		mMatConstantUniformBuffer.Create(_device, sizeof(MaterialConstants),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &_pipelineInfos.matConstants);
	}
	void VkRenderPipeline::DestroyUniformBuffers(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mObjectUniformBuffers.size(); i++)
			mObjectUniformBuffers[i].Destroy(_device);
		
		mObjectUniformBuffers.clear();

		mMatConstantUniformBuffer.Destroy(_device);
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

		if (_pipelineInfos.illumModel != IlluminationModel::None)
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

	void VkRenderPipeline::InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		for (auto it = mObjectUniformBuffers.begin(); it != mObjectUniformBuffers.end(); ++it)
			it->UpdateData(device, _data, _dataSize, _offset);
	}
	void VkRenderPipeline::UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		uint32 frameIndex = _frame.As<VkRenderFrame>().frameIndex;
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		SA_ASSERT(frameIndex >= 0u && frameIndex < mObjectUniformBuffers.size(), OutOfRange, Rendering, frameIndex, 0u, mObjectUniformBuffers.size());

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
