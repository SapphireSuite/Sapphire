// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Texture/VkCubemap.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffers/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	void Cubemap::Create(const IRenderInstance& _instance, const RawCubemap& _rawCubemap)
	{
		uint64 dataSize = SizeOf(_rawCubemap.data) * API_GetChannelNum(_rawCubemap.format) * 6u;
		uint64 irradianceSize = SizeOf(_rawCubemap.irradiancemapData) * API_GetChannelNum(_rawCubemap.format) * 6u;

		const Device& device = _instance.As<RenderInstance>().device;

		Buffer stagingBuffer;
		stagingBuffer.Create(device, dataSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			_rawCubemap.data.data());


		ImageBufferCreateInfos imageBufferCreateInfos;

		imageBufferCreateInfos.imageType = ImageType::Cube;

		imageBufferCreateInfos.format = _rawCubemap.format;
		imageBufferCreateInfos.extent = _rawCubemap.extent;

		imageBufferCreateInfos.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageBufferCreateInfos.mipLevels = _rawCubemap.mipLevels;

		if (_rawCubemap.mipLevels > 1)
			imageBufferCreateInfos.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;


		// Copy image to shader.
		ImageBuffer::TransitionInfos undefToDstTransitionInfos{};
		undefToDstTransitionInfos.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		undefToDstTransitionInfos.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		undefToDstTransitionInfos.mipLevels = _rawCubemap.mipLevels;
		undefToDstTransitionInfos.imageType = ImageType::Cube;

		ImageBuffer::TransitionInfos dstToReadTransitionInfos{};
		dstToReadTransitionInfos.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		dstToReadTransitionInfos.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		dstToReadTransitionInfos.mipLevels = _rawCubemap.mipLevels;
		dstToReadTransitionInfos.imageType = ImageType::Cube;


		// === Create buffer ===
		mBuffer.Create(device, imageBufferCreateInfos);

		mBuffer.TransitionImageLayout(device, undefToDstTransitionInfos);

		ImageBuffer::CopyBufferImageInfos copyInfos{};
		copyInfos.buffer = stagingBuffer;
		copyInfos.extent = _rawCubemap.extent;
		copyInfos.format = _rawCubemap.format;
		copyInfos.mipLevels = _rawCubemap.mipLevels;
		copyInfos.imageType = ImageType::Cube;

		mBuffer.CopyBufferToImage(device, copyInfos);

		mBuffer.TransitionImageLayout(device, dstToReadTransitionInfos);

		
		// === Create irradiance buffer ===
		imageBufferCreateInfos.mipLevels = 1u;
		imageBufferCreateInfos.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

		copyInfos.mipLevels = 1u;
		undefToDstTransitionInfos.mipLevels = 1u;
		dstToReadTransitionInfos.mipLevels = 1u;

		stagingBuffer.UpdateData(device, _rawCubemap.irradiancemapData.data(), irradianceSize);

		mIrradianceBuffer.Create(device, imageBufferCreateInfos);

		mIrradianceBuffer.TransitionImageLayout(device, undefToDstTransitionInfos);

		mIrradianceBuffer.CopyBufferToImage(device, copyInfos);

		mIrradianceBuffer.TransitionImageLayout(device, dstToReadTransitionInfos);


		stagingBuffer.Destroy(device);



		// CreateTextureSampler. // TODO: Sampler is not link to 1 image: Use 1 for multiple image!
		VkSamplerCreateInfo samplerCreateInfo{};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.pNext = nullptr;
		samplerCreateInfo.flags = 0;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.anisotropyEnable = VK_TRUE;
		samplerCreateInfo.maxAnisotropy = 16.0f;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = static_cast<float>(_rawCubemap.mipLevels);
		samplerCreateInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;

		SA_VK_ASSERT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &mSampler),
			CreationFailed, Rendering, L"Failed to create texture sampler!")
	}

	void Cubemap::Destroy(const IRenderInstance& _instance)
	{
		const Device& device = _instance.As<RenderInstance>().device;

		vkDestroySampler(device, mSampler, nullptr);
		mSampler = VK_NULL_HANDLE;

		mBuffer.Destroy(device);
		mIrradianceBuffer.Destroy(device);
	}

	VkDescriptorImageInfo Cubemap::CreateDescriptorImageInfo() const noexcept
	{
		VkDescriptorImageInfo descImage{};
		descImage.sampler = mSampler;
		descImage.imageView = mBuffer;
		descImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		return descImage;
	}

	VkDescriptorImageInfo Cubemap::CreateIrradianceDescriptorImageInfo() const noexcept
	{
		VkDescriptorImageInfo descImage{};
		descImage.sampler = mSampler;
		descImage.imageView = mIrradianceBuffer;
		descImage.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		return descImage;
	}

	VkWriteDescriptorSet Cubemap::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem) noexcept
	{
		VkWriteDescriptorSet writeDesc{};
		writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDesc.pNext = nullptr;
		writeDesc.dstSet = _descriptorSet;
		writeDesc.dstBinding = _binding;
		writeDesc.dstArrayElement = _arrayElem;
		writeDesc.descriptorCount = 1u;
		writeDesc.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		writeDesc.pImageInfo = nullptr; // Will be set in pipeline.
		writeDesc.pBufferInfo = nullptr; // Will be set in pipeline.
		writeDesc.pTexelBufferView = nullptr; // Will be set in pipeline.

		return writeDesc;
	}
}

#endif
