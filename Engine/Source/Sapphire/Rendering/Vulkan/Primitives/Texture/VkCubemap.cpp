// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Texture/VkCubemap.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkCubemap::Create(const IRenderInstance& _instance, const RawCubemap& _rawCubemap)
	{
		VkFormat format = API_GetFormat(_rawCubemap.channel);
		uint64 textureSize = _rawCubemap.GetMapSize();
		//uint64 textureSize = _rawCubemap.GetTotalSize();

		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		VkBuffer stagingBuffer;
		stagingBuffer.Create(device, textureSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			_rawCubemap.cubemapData);


		VkImageBufferCreateInfos imageBufferCreateInfos;

		imageBufferCreateInfos.format = format;
		imageBufferCreateInfos.extent = VkExtent3D{ _rawCubemap.width, _rawCubemap.height, 1 };
		imageBufferCreateInfos.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageBufferCreateInfos.mipMapLevels = RawCubemap::mipLevels;
		imageBufferCreateInfos.layerNum = 6u;
		imageBufferCreateInfos.imageViewType = VK_IMAGE_VIEW_TYPE_CUBE;
		imageBufferCreateInfos.imageFlags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		if (RawCubemap::mipLevels > 1)
			imageBufferCreateInfos.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		// Copy image to shader.
		const VkTransitionImageInfos undefToDstTransitionInfos
		{
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			RawCubemap::mipLevels,
			6u,
		};

		const VkTransitionImageInfos dstToReadTransitionInfos
		{
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			RawCubemap::mipLevels,
			6u,
		};


		// === Create buffer ===
		mBuffer.Create(device, imageBufferCreateInfos);

		mBuffer.TransitionImageLayout(device, undefToDstTransitionInfos);

		mBuffer.CopyBufferToImage(device, stagingBuffer,
			imageBufferCreateInfos.extent,
			static_cast<uint32>(_rawCubemap.channel),
			1u,
			6u);

		//mBuffer.TransitionImageLayout(device, dstToReadTransitionInfos);

		mBuffer.GenerateMipmaps(device, format, _rawCubemap.width, _rawCubemap.height, RawCubemap::mipLevels, 6u);


		/*
		// === Create irradiance buffer ===
		uint64 irradianceSize = _rawCubemap.GetIrradianceMapSize();
		imageBufferCreateInfos.extent = VkExtent3D{ _rawCubemap.GetIrradianceWidth(), _rawCubemap.GetIrradianceHeight(), 1 };

		stagingBuffer.UpdateData(device, _rawCubemap.irradiancemapData, irradianceSize);

		mIrradianceBuffer.Create(device, imageBufferCreateInfos);

		mIrradianceBuffer.TransitionImageLayout(device, undefToDstTransitionInfos);

		mIrradianceBuffer.CopyBufferToImage(device, stagingBuffer,
			imageBufferCreateInfos.extent,
			static_cast<uint32>(_rawCubemap.channel),
			1u,
			6u);

		mIrradianceBuffer.TransitionImageLayout(device, dstToReadTransitionInfos);
		*/

		stagingBuffer.Destroy(device);



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
			static_cast<float>(RawCubemap::mipLevels),					// maxLod.
			VK_BORDER_COLOR_INT_OPAQUE_BLACK,							// borderColor
			VK_FALSE,													// unnormalizedCoordinates.
		};

		SA_VK_ASSERT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &mSampler),
			CreationFailed, Rendering, L"Failed to create texture sampler!")
	}

	void VkCubemap::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		vkDestroySampler(device, mSampler, nullptr);
		mSampler = VK_NULL_HANDLE;

		mBuffer.Destroy(device);
		mIrradianceBuffer.Destroy(device);
	}

	VkDescriptorImageInfo VkCubemap::CreateDescriptorImageInfo() const noexcept
	{
		return VkDescriptorImageInfo
		{
			mSampler,											// sampler.
			mBuffer,											// imageView.
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL			// imageLayout.
		};
	}

	VkDescriptorImageInfo VkCubemap::CreateIrradianceDescriptorImageInfo() const noexcept
	{
		return VkDescriptorImageInfo
		{
			mSampler,											// sampler.
			mIrradianceBuffer,									// imageView.
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL			// imageLayout.
		};
	}

	VkWriteDescriptorSet VkCubemap::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem) noexcept
	{
		return VkWriteDescriptorSet
		{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,				// sType.
			nullptr,											// pNext.
			_descriptorSet,										// dstSet.
			_binding,											// dstBinding.
			_arrayElem,											// dstArrayElement.
			1,													// descriptorCount.
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,			// descriptorType.
			nullptr,											// pImageInfo.			// Will be set in pipeline.
			nullptr,											// pBufferInfo.
			nullptr												// pTexelBufferView.
		};
	}
}

#endif
