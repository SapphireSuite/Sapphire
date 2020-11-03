// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Pipeline/VkGraphicRenderPipeline.hpp>

#include <Rendering/Framework/Primitives/Light/LightType.hpp>
#include <Rendering/Framework/Primitives/Material/RawMaterial.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkCubemap.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkGraphicRenderPipeline::Create_Internal(const IRenderInstance& _instance, const PipelineCreateInfos& _infos)
	{
		const VkRenderInstance& vkInstance = _instance.As<VkRenderInstance>();
		const VkDevice& device = vkInstance.GetDevice();

		if(_infos.uniformBufferSize > 0u)
			CreateUniformBuffers(device, _infos, 3); // TODO Aurel: destroy image numbers

		VkRenderPipeline::Create_Internal(_instance, _infos);
	}

	void VkGraphicRenderPipeline::Destroy(const IRenderInstance& _instance)
	{
		const VkRenderInstance& vkInstance = _instance.As<VkRenderInstance>();
		const VkDevice& device = vkInstance.GetDevice();

		DestroyUniformBuffers(device);

		VkRenderPipeline::Destroy(_instance);
	}

	void VkGraphicRenderPipeline::CreateDescriptorSetLayoutBindings(const PipelineCreateInfos& _infos,
		std::vector<VkDescriptorSetLayoutBinding>& _layoutBindings) const noexcept
	{
		// Static UBO binding.
		_layoutBindings.push_back(VkDescriptorSetLayoutBinding
		{
			0,																		// binding.
			VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,										// descriptorType.
			1,																		// descriptorCount.
			VK_SHADER_STAGE_VERTEX_BIT,												// stageFlags.
			nullptr																	// pImmutableSamplers.
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
		if (_infos.textures.GetTextureNum() == 0)  // TODO: CLEAN LATER.
		{
			_layoutBindings.push_back(VkDescriptorSetLayoutBinding
			{
				2,																	// binding.
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,							// descriptorType.
				1,																	// descriptorCount.
				VK_SHADER_STAGE_FRAGMENT_BIT,										// stageFlags.
				nullptr																// pImmutableSamplers.
			});
		}
		else
		{
			_layoutBindings.push_back(VkDescriptorSetLayoutBinding
			{
				2,																	// binding.
				VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,							// descriptorType.
				_infos.textures.GetTextureNum(),									// descriptorCount.
				VK_SHADER_STAGE_FRAGMENT_BIT,										// stageFlags.
				nullptr																// pImmutableSamplers.
			});
		}


		// Light bindings.
		if (_infos.pipelineFlags & PipelineFlag::Lighting)
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
					4 + i,															// binding.
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,								// descriptorType.
					1,																// descriptorCount.
					VK_SHADER_STAGE_FRAGMENT_BIT,									// stageFlags.
					nullptr															// pImmutableSamplers.
				});
			}

			// Skybox (IBL) binding.
			if (_infos.pipelineFlags & PipelineFlag::IBL)
			{
				// Skybox.
				_layoutBindings.push_back(VkDescriptorSetLayoutBinding
				{
					7,																// binding.
					VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,						// descriptorType.
					1,																// descriptorCount.
					VK_SHADER_STAGE_FRAGMENT_BIT,									// stageFlags.
					nullptr															// pImmutableSamplers.
				});

				// Irradiance map.
				_layoutBindings.push_back(VkDescriptorSetLayoutBinding
				{
					8,																// binding.
					VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,						// descriptorType.
					1,																// descriptorCount.
					VK_SHADER_STAGE_FRAGMENT_BIT,									// stageFlags.
					nullptr															// pImmutableSamplers.
				});

				// BRDF lookup texture.
				_layoutBindings.push_back(VkDescriptorSetLayoutBinding
				{
					9,																// binding.
					VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,						// descriptorType.
					1,																// descriptorCount.
					VK_SHADER_STAGE_FRAGMENT_BIT,									// stageFlags.
					nullptr															// pImmutableSamplers.
				});
			}
		}
	}

	void VkGraphicRenderPipeline::CreateDescriptorSetLayout(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos)
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


	void VkGraphicRenderPipeline::CreateDescriptorPoolSize(const PipelineCreateInfos& _infos, uint32 _imageNum,
		std::vector<VkDescriptorPoolSize>& _poolSizes) const noexcept
	{
		// Static UBO binding.
		_poolSizes.push_back(VkDescriptorPoolSize
		{
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,										// type.
			_imageNum * 100,																// descriptorCount.
		});


		// Object UBO binding.
		if (_infos.uniformBufferSize > 0u)
		{
			_poolSizes.push_back(VkDescriptorPoolSize
			{
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// type.
				_imageNum * 100															// descriptorCount.
			});
		}


		// Texture bindings.
		_poolSizes.push_back(VkDescriptorPoolSize
		{
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,								// type.
			1 * 100,																		// descriptorCount.
		});


		// Light bindings.
		if (_infos.pipelineFlags & PipelineFlag::Lighting)
		{
			// Material UBO bindigs.
			_poolSizes.push_back(VkDescriptorPoolSize
			{
				VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,									// type.
				1 * 100,																	// descriptorCount.
			});


			// Light buffers binding.
			_poolSizes.insert(_poolSizes.end(),
				static_cast<uint8>(LightType::Max),
				VkDescriptorPoolSize
				{
					VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,								// type.
					1,																// descriptorCount.
				}
			);


			// Skybox binding (IBL).
			if (_infos.pipelineFlags & PipelineFlag::IBL)
			{
				_poolSizes.push_back(VkDescriptorPoolSize
				{
					VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,						// type.
					3,																// descriptorCount.
				});
			}
		}
	}

	void VkGraphicRenderPipeline::CreateDescriptorPool(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum)
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


	void VkGraphicRenderPipeline::CreateWriteDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _index,
		std::vector<DescriptorInfo>& _descriptorInfos,
		std::vector<VkWriteDescriptorSet>& _descriptorWrites) const noexcept
	{
		// Camera UBO binding.
		const VkGPUStorageBuffer<Camera_GPU>& cameraBuffer = _instance.GetGPUCameraBuffer();

		_descriptorInfos.push_back(cameraBuffer.CreateDescriptorBufferInfo());
		_descriptorWrites.push_back(cameraBuffer.CreateWriteDescriptorSet(mDescriptorSets[_index], 0));


		// Object UBO binding.
		if (_infos.uniformBufferSize > 0u)
		{
			_descriptorInfos.push_back(mObjectUniformBuffers[_index].CreateDescriptorBufferInfo(_infos.uniformBufferSize));
			_descriptorWrites.push_back(VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[_index], 1));
		}


		// Texture bindings.
		if (!_infos.textures.albedo && _infos.skybox) // TODO: CLEAN LATER.
		{
			_descriptorInfos.push_back(_infos.skybox->As<VkCubemap>().CreateDescriptorImageInfo());
			_descriptorWrites.push_back(VkCubemap::CreateWriteDescriptorSet(mDescriptorSets[_index], 2));
		}
		else
		{
			for (uint32 i = 0u; i < MaterialTextures::size; ++i)
			{
				if (!_infos.textures.data[i])
					continue;

				const VkTexture& vkTexture = _infos.textures.data[i]->As<VkTexture>();

				_descriptorInfos.push_back(vkTexture.CreateDescriptorImageInfo());
				_descriptorWrites.push_back(VkTexture::CreateWriteDescriptorSet(mDescriptorSets[_index], 2, i));
			}
		}


		// Light bindings.
		if (_infos.pipelineFlags & PipelineFlag::Lighting)
		{
			// Material UBO bindigs.
			_descriptorInfos.push_back(mMatConstantUniformBuffer.CreateDescriptorBufferInfo(sizeof(MaterialConstants)));
			_descriptorWrites.push_back(VkUniformBuffer::CreateWriteDescriptorSet(mDescriptorSets[_index], 3));


			// Directionnal light buffer bindigs.
			const VkGPUStorageBuffer<DirectionnalLight_GPU>& dLightBuffer = _instance.GetGPUDirectionnalLightBuffer();

			_descriptorInfos.push_back(dLightBuffer.CreateDescriptorBufferInfo());
			_descriptorWrites.push_back(dLightBuffer.CreateWriteDescriptorSet(mDescriptorSets[_index], 4));

			// Point light buffer bindigs.
			const VkGPUStorageBuffer<PointLight_GPU>& pLightBuffer = _instance.GetGPUPointLightBuffer();

			_descriptorInfos.push_back(pLightBuffer.CreateDescriptorBufferInfo());
			_descriptorWrites.push_back(pLightBuffer.CreateWriteDescriptorSet(mDescriptorSets[_index], 5));



			// Spot light buffer bindigs.
			const VkGPUStorageBuffer<SpotLight_GPU>& sLightBuffer = _instance.GetGPUSpotLightBuffer();

			_descriptorInfos.push_back(sLightBuffer.CreateDescriptorBufferInfo());
			_descriptorWrites.push_back(sLightBuffer.CreateWriteDescriptorSet(mDescriptorSets[_index], 6));


			if (_infos.pipelineFlags & PipelineFlag::IBL && _infos.skybox)
			{
				_descriptorInfos.push_back(_infos.skybox->As<VkCubemap>().CreateDescriptorImageInfo());
				_descriptorWrites.push_back(VkCubemap::CreateWriteDescriptorSet(mDescriptorSets[_index], 7));

				_descriptorInfos.push_back(_infos.skybox->As<VkCubemap>().CreateIrradianceDescriptorImageInfo());
				_descriptorWrites.push_back(VkCubemap::CreateWriteDescriptorSet(mDescriptorSets[_index], 8));

				_descriptorInfos.push_back(ITexture::brdfLUT->As<VkTexture>().CreateDescriptorImageInfo());
				_descriptorWrites.push_back(VkTexture::CreateWriteDescriptorSet(mDescriptorSets[_index], 9));
			}
		}
	}

	void VkGraphicRenderPipeline::CreateDescriptorSets(const VkRenderInstance& _instance, const PipelineCreateInfos& _infos, uint32 _imageNum)
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


	void VkGraphicRenderPipeline::CreateUniformBuffers(const VkDevice& _device, const PipelineCreateInfos& _infos, uint32 _imageNum)
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
	void VkGraphicRenderPipeline::DestroyUniformBuffers(const VkDevice& _device)
	{
		for (uint32 i = 0; i < mObjectUniformBuffers.size(); i++)
			mObjectUniformBuffers[i].Destroy(_device);
		
		mObjectUniformBuffers.clear();

		mMatConstantUniformBuffer.Destroy(_device);
	}


	void VkGraphicRenderPipeline::InitData(const IRenderInstance& _instance, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		for (auto it = mObjectUniformBuffers.begin(); it != mObjectUniformBuffers.end(); ++it)
			it->UpdateData(device, _data, _dataSize, _offset);
	}
	void VkGraphicRenderPipeline::UpdateData(const IRenderInstance& _instance, const IRenderFrame& _frame, const void* _data, uint32 _dataSize, uint32 _offset)
	{
		uint32 frameIndex = _frame.As<VkRenderFrame>().index;
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		SA_ASSERT(frameIndex >= 0u && frameIndex < SizeOf(mObjectUniformBuffers), OutOfRange, Rendering, frameIndex, 0u, SizeOf(mObjectUniformBuffers));

		mObjectUniformBuffers[frameIndex].UpdateData(device, _data, _dataSize, _offset);
	}
}

#endif
