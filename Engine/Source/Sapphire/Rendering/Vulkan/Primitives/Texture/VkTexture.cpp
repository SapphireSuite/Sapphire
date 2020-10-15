// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Primitives/Texture/VkTexture.hpp>

#include <Rendering/Vulkan/System/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void VkTexture::Create(const IRenderInstance& _instance, const RawTexture& _rawTexture)
	{
		VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;// TODO: use texture channel!
		uint32 imageSize = _rawTexture.width * _rawTexture.height * 4; // TODO: use texture channel!

		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		VkBuffer stagingBuffer;
		stagingBuffer.Create(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(device, stagingBuffer, 0, imageSize, 0, &data);
		memcpy(data, _rawTexture.data, imageSize);
		vkUnmapMemory(device, stagingBuffer);

		uint32 mipLevels = ComputeMipMapLevels(_rawTexture.width, _rawTexture.height);
		VkExtent3D textureExtent{ _rawTexture.width, _rawTexture.height, 1 };

		const VkImageBufferCreateInfos imageBufferCreateInfos
		{
			format,														// format.
			textureExtent,												// extent.
			
			VK_IMAGE_USAGE_TRANSFER_SRC_BIT |							// usage.
			VK_IMAGE_USAGE_TRANSFER_DST_BIT |
			VK_IMAGE_USAGE_SAMPLED_BIT,

			mipLevels,													// mipMapLevels.
		};

		mBuffer.Create(device, imageBufferCreateInfos);


		// Copy image to shader.
		const VkTransitionImageInfos undefToDstTransitionInfos
		{
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			mipLevels,
		};

		mBuffer.TransitionImageLayout(device, undefToDstTransitionInfos);

		mBuffer.CopyBufferToImage(device, stagingBuffer, textureExtent);
		stagingBuffer.Destroy(device);

		if (mipLevels > 1)
		{
			// Will transition image layout as read only at the end.
			mBuffer.GenerateMipmaps(device, format, _rawTexture.width, _rawTexture.height, mipLevels);
			// TODO: Compute mipmap only once and save mipmap levels in TextureAsset.
		}
		else
		{
			const VkTransitionImageInfos dstToReadTransitionInfos
			{
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				mipLevels,
			};

			mBuffer.TransitionImageLayout(device, dstToReadTransitionInfos);
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
			static_cast<float>(mipLevels),								// maxLod.
			VK_BORDER_COLOR_INT_OPAQUE_BLACK,							// borderColor
			VK_FALSE,													// unnormalizedCoordinates.
		};

		SA_VK_ASSERT(vkCreateSampler(device, &samplerCreateInfo, nullptr, &mSampler),
			CreationFailed, Rendering, L"Failed to create texture sampler!")
	}

	void VkTexture::Destroy(const IRenderInstance& _instance)
	{
		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		vkDestroySampler(device, mSampler, nullptr);
		mSampler = VK_NULL_HANDLE;

		mBuffer.Destroy(device);
	}

	VkDescriptorImageInfo VkTexture::CreateDescriptorImageInfo() const noexcept
	{
		return VkDescriptorImageInfo
		{
			mSampler,											// sampler.
			mBuffer,											// imageView.
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL			// imageLayout.
		};
	}
	VkWriteDescriptorSet VkTexture::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding, uint32 _arrayElem) noexcept
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
