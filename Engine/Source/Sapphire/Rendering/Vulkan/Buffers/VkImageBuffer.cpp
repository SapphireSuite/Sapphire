// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkImageBuffer.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

namespace Sa::Vk
{
	bool ImageBuffer::IsValid() const noexcept
	{
		return mImage != VK_NULL_HANDLE && mImageView != VK_NULL_HANDLE && mImageMemory != VK_NULL_HANDLE;
	}

	void ImageBuffer::CreateImage(const Device& _device, const VkImageCreateInfo& _vkInfos)
	{
		// Create Image.
		SA_VK_ASSERT(vkCreateImage(_device, &_vkInfos, nullptr, &mImage),
			CreationFailed, Rendering, L"Failed to create image!");


		// Create Image memory.
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(_device, mImage, &memRequirements);

		uint32 memoryTypeIndex = Buffer::FindMemoryType(_device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkMemoryAllocateInfo memoryAllocInfo{};
		memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocInfo.pNext = nullptr;
		memoryAllocInfo.allocationSize = memRequirements.size;
		memoryAllocInfo.memoryTypeIndex = memoryTypeIndex;

		SA_VK_ASSERT(vkAllocateMemory(_device, &memoryAllocInfo, nullptr, &mImageMemory),
			MemoryAllocationFailed, Rendering, L"Failed to allocate image memory!");

		vkBindImageMemory(_device, mImage, mImageMemory, 0);
	}

	void ImageBuffer::CreateImageView(const Device& _device, const VkImageViewCreateInfo& _vkInfos)
	{
		SA_VK_ASSERT(vkCreateImageView(_device, &_vkInfos, nullptr, &mImageView),
			CreationFailed, Rendering, L"Failed to create image view!");
	}

	void ImageBuffer::Create(const Device& _device, const ImageBufferCreateInfos& _infos)
	{
		CreateImage(_device, _infos.VkImageInfos());
		CreateImageView(_device, _infos.VkImageViewInfos(mImage));
	}
	
	void ImageBuffer::CreateFromImage(const Device& _device, const ImageBufferCreateInfos& _infos, VkImage& _image)
	{
		mImage = _image;

		CreateImageView(_device, _infos.VkImageViewInfos(_image));
	}
	
	void ImageBuffer::Destroy(const Device& _device)
	{
		vkDestroyImageView(_device, mImageView, nullptr);
		mImageView = VK_NULL_HANDLE;

		// In case, the image was given to the buffer (using CreateFromImage).
		if (mImageMemory != VK_NULL_HANDLE)
		{
			vkDestroyImage(_device, mImage, nullptr);
			mImage = VK_NULL_HANDLE;

			// Free memory after destroying image: memory no more used.
			vkFreeMemory(_device, mImageMemory, nullptr);
			mImageMemory = VK_NULL_HANDLE;
		}
	}


	void ImageBuffer::TransitionImageLayout(const Device& _device, const ImageBuffer::TransitionInfos& _infos)
	{
		CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_device, QueueType::Graphics);


		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.pNext = nullptr;
		barrier.srcAccessMask = 0; // Set later.
		barrier.dstAccessMask = 0; // Set later.
		barrier.oldLayout = _infos.oldLayout;
		barrier.newLayout = _infos.newLayout;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = mImage;
		barrier.subresourceRange.aspectMask = _infos.aspectFlags;
		barrier.subresourceRange.baseMipLevel = 0u;
		barrier.subresourceRange.levelCount = _infos.mipLevels;
		barrier.subresourceRange.baseArrayLayer = 0u;
		barrier.subresourceRange.layerCount = API_GetLayerNum(_infos.imageType);

		VkPipelineStageFlags srcStage = 0;
		VkPipelineStageFlags dstStage = 0;

		// Set stages and access mask.
		if (_infos.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED)
		{
			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			if (_infos.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (_infos.newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

				dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			}
		}
		else if (_infos.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			if (_infos.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
		}
		else
			SA_ASSERT(false, NotSupported, Rendering, L"Unsupported image layout transition!");

		vkCmdPipelineBarrier(commandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

		CommandBuffer::EndSingleTimeCommands(_device, commandBuffer);
	}
	
	void ImageBuffer::CopyBufferToImage(const Device& _device, const ImageBuffer::CopyBufferImageInfos& _infos)
	{
		CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_device, QueueType::Transfer);

		uint64 offset = 0u;
		VkExtent3D extent{ _infos.extent.x, _infos.extent.y, 1 };
		uint32 layerCount = API_GetLayerNum(_infos.imageType);
		uint32 channelNum = API_GetChannelNum(_infos.format);

		std::vector<VkBufferImageCopy> bufferImageCopies;

		for (uint32 currMipLevel = 0u; currMipLevel < _infos.mipLevels; ++currMipLevel)
		{
			VkBufferImageCopy imageCopy{};
			imageCopy.bufferOffset = offset;
			imageCopy.bufferRowLength = 0u;
			imageCopy.bufferImageHeight = 0u;

			imageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageCopy.imageSubresource.mipLevel = currMipLevel;
			imageCopy.imageSubresource.baseArrayLayer = 0u;
			imageCopy.imageSubresource.layerCount = layerCount;

			imageCopy.imageOffset = VkOffset3D{ 0, 0, 0 };
			imageCopy.imageExtent = extent;

			bufferImageCopies.emplace_back(imageCopy);

			offset += extent.width * extent.height * channelNum;

			if (extent.width > 1)
				extent.width >>= 1;

			if (extent.height > 1)
				extent.height >>= 1;
		}

		vkCmdCopyBufferToImage(commandBuffer, _infos.buffer, mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, SizeOf(bufferImageCopies), bufferImageCopies.data());


		CommandBuffer::EndSingleTimeCommands(_device, commandBuffer);
	}

	/*
	void ImageBuffer::GenerateMipmaps(const Device& _device, VkFormat format, uint32 _width, uint32 _height, uint32 _mipLevels, uint32 _layerCount)
	{
#if SA_DEBUG
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(_device, format, &formatProperties);

		// Image format supports linear blitting.
		SA_ASSERT(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
			NotSupported, Rendering, L"Texture image format does not support linear blitting!");
#endif

		CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_device, QueueType::Graphics);

		VkImageMemoryBarrier barrier
		{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,							// sType.
			nullptr,														// pNext.
			0, // Set later.												// srcAccessMask.
			0, // Set later.												// dstAccessMask.
			VK_IMAGE_LAYOUT_UNDEFINED, // Set later.						// oldLayout.
			VK_IMAGE_LAYOUT_UNDEFINED, // Set later.						// newLayout.
			VK_QUEUE_FAMILY_IGNORED,										// srcQueueFamilyIndex.
			VK_QUEUE_FAMILY_IGNORED,										// dstQueueFamilyIndex.
			mImage,															// image.

			VkImageSubresourceRange											// subresourceRange.
			{
				VK_IMAGE_ASPECT_COLOR_BIT,						// aspectMask.
				0,												// baseMipLevel.
				1,												// levelCount.
				0,												// baseArrayLayer.
				_layerCount,									// layerCount.
			}
		};

		int32 mipWidth = _width;
		int32 mipHeight = _height;

		for (uint32 currMipLevel = 1u; currMipLevel < _mipLevels; currMipLevel++)
		{
			barrier.subresourceRange.baseMipLevel = currMipLevel - 1;

			// Transfer prev mip level to source.
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
				0, 0, nullptr, 0, nullptr, 1, &barrier);


			// Handles non-squared images.
			int32 dstMipWidth = mipWidth > 1 ? mipWidth / 2 : 1;
			int32 dstMipHeight = mipHeight > 1 ? mipHeight / 2 : 1;

			const VkImageBlit blit
			{
				VkImageSubresourceLayers								// srcSubresource.
				{
					VK_IMAGE_ASPECT_COLOR_BIT,					// aspectMask.
					currMipLevel - 1,							// mipLevel.
					0,											// baseArrayLayer.
					_layerCount,								// layerCount.
				},

				{														// srcOffsets[2].
					VkOffset3D { 0, 0, 0 },
					VkOffset3D { mipWidth, mipHeight, 1 }
				},


				VkImageSubresourceLayers								// dstSubresource.
				{
					VK_IMAGE_ASPECT_COLOR_BIT,					// aspectMask.
					currMipLevel,								// mipLevel.
					0,											// baseArrayLayer.
					_layerCount,								// layerCount.
				},

				{														// dstOffsets[2].
					VkOffset3D { 0, 0, 0 },
					VkOffset3D { dstMipWidth, dstMipHeight, 1 }
				},
			};

			vkCmdBlitImage(commandBuffer,
				mImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit, VK_FILTER_LINEAR);


			// Transfer pev mip level to read only.
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			vkCmdPipelineBarrier(commandBuffer,
				VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
				0, nullptr,
				0, nullptr,
				1, &barrier);


			mipWidth = dstMipWidth;
			mipHeight = dstMipHeight;
		}


		// Transfer last mip level to read only.
		barrier.subresourceRange.baseMipLevel = _mipLevels - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		CommandBuffer::EndSingleTimeCommands(_device, commandBuffer);
	}
	*/


	ImageBuffer::operator VkImage() const noexcept
	{
		return mImage;
	}
	
	ImageBuffer::operator VkImageView() const noexcept
	{
		return mImageView;
	}
	
	ImageBuffer::operator VkDeviceMemory() const noexcept
	{
		return mImageMemory;
	}
}
