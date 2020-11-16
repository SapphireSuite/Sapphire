// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Material/VkMaterial.hpp>

#include <Core/Algorithms/SizeOf.hpp>
#include <Core/Types/Variadics/Pair.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

#include <Rendering/Vulkan/Primitives/Pipeline/VkPipeline.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>
#include <Rendering/Vulkan/Primitives/Texture/VkCubemap.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
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

	void Material::Bind(const RenderFrame& _frame, const IPipeline& _pipeline)
	{
		Vk::CommandBuffer& commandBuffer = _frame.buffer.As<Vk::FrameBuffer>().commandBuffer;

		const Pipeline& vkPipeline = _pipeline.As<Pipeline>();

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline.GetLayout(),
			0, 1, &mDescriptorSets[_frame.index % SizeOf(mDescriptorSets)], 0, nullptr);
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
			VkDescriptorType type = API_GetDescriptorType(it->GetType());

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
		descriptorPoolInfo.maxSets = _infos.descriptorSetNum;
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

		mDescriptorSets.resize(_infos.descriptorSetNum);
		std::vector<VkDescriptorSetLayout> descriptorSetLayouts(_infos.descriptorSetNum, vkPipeline.GetDescriptorSetLayout());

		VkDescriptorSetAllocateInfo descriptorSetAllocInfo{};
		descriptorSetAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocInfo.pNext = nullptr;
		descriptorSetAllocInfo.descriptorPool = mDescriptorPool;
		descriptorSetAllocInfo.descriptorSetCount = _infos.descriptorSetNum;
		descriptorSetAllocInfo.pSetLayouts = descriptorSetLayouts.data();

		SA_VK_ASSERT(vkAllocateDescriptorSets(_device, &descriptorSetAllocInfo, mDescriptorSets.data()),
			MemoryAllocationFailed, Rendering, L"Failed to allocate descriptor sets!");


		UpdateDescriptorSets(_device, _infos.bindings);
	}
	
	void Material::UpdateDescriptorSets(const Device& _device, const std::vector<MaterialBindingInfos>& _bindings)
	{
		// Query allocation size.
		uint32 bufferDescSize = 0u;
		uint32 imageDescSize = 0u;

		for (auto it = _bindings.begin(); it != _bindings.end(); ++it)
			CountDescriptors(*it, bufferDescSize, imageDescSize);

		// Allocate.
		std::vector<VkDescriptorBufferInfo> bufferDescs;
		bufferDescs.reserve(bufferDescSize);

		std::vector<VkDescriptorImageInfo> imageDescs;
		imageDescs.reserve(imageDescSize);

		std::vector<VkWriteDescriptorSet> descWrites;
		descWrites.reserve(bufferDescSize + imageDescSize);

		for (auto it = _bindings.begin(); it != _bindings.end(); ++it)
			FillDescriptorWrites(*it, bufferDescs, imageDescs, descWrites);

		vkUpdateDescriptorSets(_device, SizeOf(descWrites), descWrites.data(), 0, nullptr);
	}

	void Material::CountDescriptors(const MaterialBindingInfos& _binding, uint32& _bufferDescSize, uint32& _imageDescSize) const noexcept
	{
		ShaderBindingType type = _binding.GetType();
		const uint32 descSetSize = _binding.descriptor == uint32(-1) ? SizeOf(mDescriptorSets) : 1u;

		if (type == ShaderBindingType::UniformBuffer)
			_bufferDescSize += SizeOf(_binding.GetUniformBuffers()) * descSetSize;
		else if (type == ShaderBindingType::ImageSampler2D || type == ShaderBindingType::ImageSamplerCube)
			_imageDescSize += SizeOf(_binding.GetImageSamplers2D()) * descSetSize;
		else if (type == ShaderBindingType::StorageBuffer)
			_bufferDescSize += descSetSize;
		else if (type == ShaderBindingType::InputAttachment)
			_imageDescSize += descSetSize;
		else
			SA_LOG("ShaderBindingType not supported yet!", Warning, Rendering);
	}

	void Material::FillDescriptorWrites(const MaterialBindingInfos& _binding,
		std::vector<VkDescriptorBufferInfo>& _bufferDescs,
		std::vector<VkDescriptorImageInfo>& _imageDescs,
		std::vector<VkWriteDescriptorSet>& _descWrites,
		uint32 _descIndex) const noexcept
	{
		// Selected descriptor index.
		if (_descIndex == uint32(-1))
		{
			if (_binding.descriptor != uint32(-1))
				_descIndex = _binding.descriptor;
			else
			{
				// Update for all descriptors.
				for (uint32 i = 0; i < SizeOf(mDescriptorSets); ++i)
					FillDescriptorWrites(_binding, _bufferDescs, _imageDescs, _descWrites, i);

				return;
			}
		}

		const ShaderBindingType bindType = _binding.GetType();

		VkWriteDescriptorSet mainWriteDesc{};
		mainWriteDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		mainWriteDesc.pNext = nullptr;
		mainWriteDesc.dstSet = mDescriptorSets[_descIndex];
		mainWriteDesc.dstBinding = _binding.binding;
		mainWriteDesc.dstArrayElement = 0u;
		mainWriteDesc.descriptorCount = 1u;
		mainWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;

		if (bindType == ShaderBindingType::UniformBuffer)
		{
			mainWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

			const std::vector<const IBuffer*>& ubos = _binding.GetUniformBuffers();

			for (uint32 j = 0; j < SizeOf(ubos); ++j)
			{
				VkWriteDescriptorSet& writeDesc = _descWrites.emplace_back(mainWriteDesc);
				writeDesc.dstArrayElement = j;

				VkDescriptorBufferInfo& desc = _bufferDescs.emplace_back(VkDescriptorBufferInfo{ ubos[j]->As<Buffer>().CreateDescriptorBufferInfo() });
				writeDesc.pBufferInfo = &desc;
			}
		}
		else if (bindType == ShaderBindingType::ImageSampler2D)
		{
			mainWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

			const std::vector<const ITexture*>& samplers = _binding.GetImageSamplers2D();

			for (uint32 j = 0u; j < SizeOf(samplers); ++j)
			{
				VkWriteDescriptorSet& writeDesc = _descWrites.emplace_back(mainWriteDesc);
				writeDesc.dstArrayElement = j;

				VkDescriptorImageInfo& desc = _imageDescs.emplace_back(VkDescriptorImageInfo{ samplers[j]->As<Texture>().CreateDescriptorImageInfo() });
				writeDesc.pImageInfo = &desc;
			}
		}
		else if (bindType == ShaderBindingType::ImageSamplerCube)
		{
			mainWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

			const std::vector<const ICubemap*>& samplers = _binding.GetImageSamplerCubes();

			for (uint32 j = 0u; j < SizeOf(samplers); ++j)
			{
				VkWriteDescriptorSet& writeDesc = _descWrites.emplace_back(mainWriteDesc);
				writeDesc.dstArrayElement = j;

				VkDescriptorImageInfo& desc = _imageDescs.emplace_back(VkDescriptorImageInfo{ samplers[j]->As<Cubemap>().CreateDescriptorImageInfo() });
				writeDesc.pImageInfo = &desc;
			}
		}
		else if (bindType == ShaderBindingType::StorageBuffer)
		{
			mainWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

			const IBuffer& storageBuffer = _binding.GetStorageBuffer();

			VkWriteDescriptorSet& writeDesc = _descWrites.emplace_back(mainWriteDesc);
			writeDesc.dstArrayElement = 0;

			VkDescriptorBufferInfo& desc = _bufferDescs.emplace_back(VkDescriptorBufferInfo{ storageBuffer.As<Buffer>().CreateDescriptorBufferInfo() });
			writeDesc.pBufferInfo = &desc;
		}
		else if (bindType == ShaderBindingType::InputAttachment)
		{
			mainWriteDesc.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;

			VkWriteDescriptorSet& writeDesc = _descWrites.emplace_back(mainWriteDesc);
			writeDesc.pImageInfo = &_imageDescs.emplace_back(VkDescriptorImageInfo{ _binding.GetInputBuffer().As<ImageBuffer>().CreateDescriptorImageInfo() });
		}
		else
			SA_LOG("ShaderBindingType for descriptor write not supported.", Error, Rendering);
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
