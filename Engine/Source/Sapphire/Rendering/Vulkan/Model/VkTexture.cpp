// Copyright 2020 Sapphire development team. All Rights Reserved.

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Rendering/Vulkan/Model/VkTexture.hpp>

#include <Rendering/Vulkan/VkRenderInstance.hpp>
#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>
#include <Sapphire/Rendering/Vulkan/Queue/VkCommandBuffer.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa
{
	void TransitionImageLayout(const Sa::VkDevice& _device, VkImage _image, VkFormat _format, VkImageLayout _oldLayout, VkImageLayout _newLayout)
	{
		Sa::VkCommandBuffer commandBuffer = VkCommandBuffer::BeginSingleTimeCommands(_device);


		VkImageMemoryBarrier barrier
		{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,							// sType.
			nullptr,														// pNext.
			0,																// srcAccessMask.
			0,																// dstAccessMask.
			_oldLayout,														// oldLayout.
			_newLayout,														// newLayout.
			VK_QUEUE_FAMILY_IGNORED,										// srcQueueFamilyIndex.
			VK_QUEUE_FAMILY_IGNORED,										// dstQueueFamilyIndex.
			_image,															// image.

			VkImageSubresourceRange											// subresourceRange.
			{
				VK_IMAGE_ASPECT_COLOR_BIT,						// aspectMask.
				0,												// baseMipLevel.
				1,												// levelCount.
				0,												// baseArrayLayer.
				1,												// layerCount.
			}
		};

		VkPipelineStageFlags srcStage = 0;
		VkPipelineStageFlags dstStage = 0;

		if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && _newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && _newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
			SA_ASSERT(false, NotSupported, Rendering, L"Unsupported image layout transition!")

		vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);


		VkCommandBuffer::EndSingleTimeCommands(_device, commandBuffer);
	}

	void CopyBufferToImage(const Sa::VkDevice& _device, VkBuffer _buffer, VkImage _image, uint32 _width, uint32 _height)
	{
		Sa::VkCommandBuffer commandBuffer = VkCommandBuffer::BeginSingleTimeCommands(_device);


		const VkBufferImageCopy bufferImageCopy
		{
			0,													// bufferOffset.
			0,													// bufferRowLength.
			0,													// bufferImageHeight.

			VkImageSubresourceLayers							// imageSubresource.
			{
				VK_IMAGE_ASPECT_COLOR_BIT,					// aspectMask.
				0,											// mipLevel.
				0,											// baseArrayLayer.
				1,											// layerCount.
			},

			{0, 0, 0},											// imageOffset.

			VkExtent3D { _width, _height, 1 }					// imageExtent.
		};

		vkCmdCopyBufferToImage(commandBuffer, _buffer, _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);


		VkCommandBuffer::EndSingleTimeCommands(_device, commandBuffer);
	}

	void VkTexture::Create(const IRenderInstance& _instance, const std::string& _fileName)
	{
		int32 texWidth = 0;
		int32 texHeight = 0;
		int32 texChannels = 0;

		stbi_uc* pixels = stbi_load(_fileName.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		uint32 imageSize = texWidth * texHeight * 4;

		SA_ASSERT(pixels, InvalidParam, Rendering, L"Failed to load texture image!");

		const VkDevice& device = _instance.As<VkRenderInstance>().GetDevice();

		VkBuffer stagingBuffer;
		stagingBuffer.Create(device, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(device, stagingBuffer, 0, imageSize, 0, &data);
		memcpy(data, pixels, imageSize);
		vkUnmapMemory(device, stagingBuffer);

		stbi_image_free(pixels);

		const VkImageCreateInfo imageCreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,					// sType.
			nullptr,												// pNext.
			0,														// flags.
			VK_IMAGE_TYPE_2D,										// imageType.
			VK_FORMAT_R8G8B8A8_SRGB,								// format.
			
			VkExtent3D												// extent.
			{
				static_cast<uint32>(texWidth),
				static_cast<uint32>(texHeight),
				1
			},
			
			1,														// mipLevels.
			1,														// arrayLayers.
			VK_SAMPLE_COUNT_1_BIT,									// samples.
			VK_IMAGE_TILING_OPTIMAL,								// tiling.

			VK_IMAGE_USAGE_TRANSFER_DST_BIT |						// usage.
			VK_IMAGE_USAGE_SAMPLED_BIT,

			VK_SHARING_MODE_EXCLUSIVE,								// sharingMode.
			VkQueueFamilyIndices::familyNum,						// queueFamilyIndexCount.
			device.GetQueueFamilyIndices().GetFamilies(),			// pQueueFamilyIndices.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
		};

		SA_VK_ASSERT(vkCreateImage(device, &imageCreateInfo, nullptr, &mImage),
			CreationFailed, Rendering, L"Failed to create image!");


		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, mImage, &memRequirements);

		uint32 memoryTypeIndex = VkBuffer::FindMemoryType(device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		const VkMemoryAllocateInfo memoryAllocInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,					// sType.
			nullptr,												// pNext.
			memRequirements.size,									// allocationSize.
			memoryTypeIndex,										// memoryTypeIndex.
		};

		SA_VK_ASSERT(vkAllocateMemory(device, &memoryAllocInfo, nullptr, &mImageMemory),
			MemoryAllocationFailed, Rendering, L"Failed to allocate image memory!");

		vkBindImageMemory(device, mImage, mImageMemory, 0);

		// Copy image to shader.
		TransitionImageLayout(device, mImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		
		CopyBufferToImage(device, stagingBuffer, mImage, static_cast<uint32>(texWidth), static_cast<uint32>(texHeight));

		TransitionImageLayout(device, mImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


		// Destroy buffers.
		stagingBuffer.Destroy(device);


		// Create Image view.
		const VkImageViewCreateInfo imageViewCreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,									// sType.
			nullptr,																	// pNext.
			VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT,					// flags.
			mImage,																		// image.
			VK_IMAGE_VIEW_TYPE_2D,														// viewType.
			VK_FORMAT_R8G8B8A8_SRGB,													// format.

			VkComponentMapping															// components.
			{
				VK_COMPONENT_SWIZZLE_IDENTITY,										// r.
				VK_COMPONENT_SWIZZLE_IDENTITY,										// g.
				VK_COMPONENT_SWIZZLE_IDENTITY,										// b.
				VK_COMPONENT_SWIZZLE_IDENTITY										// a.
			},
			VkImageSubresourceRange														// subresourceRange.
			{
				VK_IMAGE_ASPECT_COLOR_BIT,											// aspectMask.
				0,																	// baseMipLevel.
				1,																	// levelCount.
				0,																	// baseArrayLayer.
				1																	// layerCount.
			}
		};

		SA_VK_ASSERT(vkCreateImageView(device, &imageViewCreateInfo, nullptr, &mImageView),
			CreationFailed, Rendering, L"Failed to create image views!");


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
			0.0f,														// maxLod.
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

		vkDestroyImageView(device, mImageView, nullptr);
		mImageView = VK_NULL_HANDLE;

		vkDestroyImage(device, mImage, nullptr);
		mImage = VK_NULL_HANDLE;

		vkFreeMemory(device, mImageMemory, nullptr); // Free memory after destroying image: memory no more used.
		mImageMemory = VK_NULL_HANDLE;
	}

	VkWriteDescriptorSet VkTexture::CreateWriteDescriptorSet(VkDescriptorSet _descriptorSet, uint32 _binding) const noexcept
	{
		const VkDescriptorImageInfo descriptorInfo
		{
			mSampler,											// sampler.
			mImageView,											// imageView.
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL			// imageLayout.
		};

		return VkWriteDescriptorSet
		{
			VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,				// sType.
			nullptr,											// pNext.
			_descriptorSet,										// dstSet.
			_binding,											// dstBinding.
			0,													// dstArrayElement.
			1,													// descriptorCount.
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,			// descriptorType.
			&descriptorInfo,								// pImageInfo.
			nullptr,											// pBufferInfo.
			nullptr												// pTexelBufferView.
		};
	}
}

#endif
