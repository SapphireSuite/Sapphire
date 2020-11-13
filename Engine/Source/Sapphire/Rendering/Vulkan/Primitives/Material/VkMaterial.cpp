// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Material/VkMaterial.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#include <Rendering/Vulkan/Primitives/Pipeline/VkPipeline.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

#if SA_RENDERING_API == SA_VULKAN

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


	void Material::Create(const IRenderInstance& _instance, const MaterialCreateInfos& _infos)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		CreateDescriptorPool(device, _infos);
		CreateDescriptorSets(device, _infos);
	}
	
	void Material::Destroy(const IRenderInstance& _instance)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		DestroyDescriptorSets(device);
		DestroyDescriptorPool(device);
	}

	void Material::Bind(const FrameInfos& _frameInfos, const IPipeline& _pipeline)
	{
		Vk::CommandBuffer& commandBuffer = _frameInfos.frameBuffer.As<Vk::FrameBuffer>().commandBuffer;

		const Pipeline& vkPipeline = _pipeline.As<Pipeline>();

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline.GetLayout(),
			0, 1, &mDescriptorSets[_frameInfos.frameIndex], 0, nullptr);
	}


	void Material::CreateDescriptorPool(const Device& _device, const MaterialCreateInfos& _infos)
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
		descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolInfo.maxSets = imageNum;
		descriptorPoolInfo.poolSizeCount = SizeOf(poolSizes);
		descriptorPoolInfo.pPoolSizes = poolSizes.data();

		SA_VK_ASSERT(vkCreateDescriptorPool(_device, &descriptorPoolInfo, nullptr, &mDescriptorPool),
			CreationFailed, Rendering, L"Failed to create descriptor pool!");
	}
	
	void Material::DestroyDescriptorPool(const Device& _device)
	{
		vkDestroyDescriptorPool(_device, mDescriptorPool, nullptr);
		mDescriptorPool = VK_NULL_HANDLE;
	}


	void Material::CreateDescriptorSets(const Device& _device, const MaterialCreateInfos& _infos)
	{
		const Pipeline& vkPipeline = _infos.pipeline.As<Pipeline>();

		mDescriptorSets.resize(imageNum);
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(imageNum, vkPipeline.GetDescriptorSetLayout());

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
	
	void Material::UpdateDescriptorSets(const Device& _device, const MaterialBindingInfos& _binding)
	{
		std::vector<VkDescriptorBufferInfo> descBufferInfos;
		descBufferInfos.reserve(SizeOf(mDescriptorSets) * SizeOf(_binding.buffers));

		std::vector<VkDescriptorImageInfo> descImageInfos;
		descImageInfos.reserve(SizeOf(mDescriptorSets) * SizeOf(_binding.buffers));

		std::vector<VkWriteDescriptorSet> descWrites;
		descWrites.reserve(SizeOf(mDescriptorSets) * SizeOf(_binding.buffers));


		for (uint32 i = 0; i < SizeOf(mDescriptorSets); ++i)
		{
			for (uint32 j = 0u; j < SizeOf(_binding.buffers); ++j)
			{
				VkWriteDescriptorSet& writeDesc = descWrites.emplace_back();
				writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				writeDesc.pNext = nullptr;
				writeDesc.dstSet = mDescriptorSets[i];
				writeDesc.dstBinding = _binding.binding;
				writeDesc.dstArrayElement = j;
				writeDesc.descriptorCount = 1u;

				if (_binding.type == ShaderBindingType::UniformBuffer)
				{
					writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

					VkDescriptorBufferInfo& desc = descBufferInfos.emplace_back(VkDescriptorBufferInfo{ _binding.buffers[j]->As<Buffer>().CreateDescriptorBufferInfo(_binding.bufferDataSize) });
					writeDesc.pBufferInfo = &desc;
				}
				else if (_binding.type == ShaderBindingType::StorageBuffer)
				{
					writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

					// TODO: IMPLEMENT.
				}
				else if (_binding.type == ShaderBindingType::ImageSampler2D || _binding.type == ShaderBindingType::ImageSamplerCube)
				{
					writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

					VkDescriptorImageInfo& desc = descImageInfos.emplace_back(VkDescriptorImageInfo{ _binding.buffers[j]->As<Texture>().CreateDescriptorImageInfo() });
					writeDesc.pImageInfo = &desc;
				}
				else if (_binding.type != ShaderBindingType::InputAttachment)
					SA_LOG("ShaderBindingType for descriptor write not supported.", Error, Rendering);
			}
		}

		vkUpdateDescriptorSets(_device, SizeOf(descWrites), descWrites.data(), 0, nullptr);
	}

	void Material::UpdateDescriptorSets(const Device& _device, const std::vector<MaterialBindingInfos>& _bindings)
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

	void Material::DestroyDescriptorSets(const Device& _device)
	{
		(void)_device;

		// Not needed when vkDestroyDescriptorPool() is called. Otherwise, requiere VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT flag.
		//vkFreeDescriptorSets(_device, mDescriptorPool, static_cast<uint32>(mDescriptorSets.size()), mDescriptorSets.data());
		mDescriptorSets.clear();
	}
}

#endif
