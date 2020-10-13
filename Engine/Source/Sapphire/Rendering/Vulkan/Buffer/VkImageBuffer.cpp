// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffer/VkImageBuffer.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

namespace Sa
{
	bool VkImageBuffer::IsValid()
	{
		return mImage != VK_NULL_HANDLE && mImageMemory != VK_NULL_HANDLE && mImageView != VK_NULL_HANDLE;
	}

	void VkImageBuffer::Create(const VkDevice& _device, const VkImageBufferCreateInfos& _createInfos)
	{
		// Create Image.
		const VkImageCreateInfo imageCreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,					// sType.
			nullptr,												// pNext.
			0,														// flags.
			VK_IMAGE_TYPE_2D,										// imageType.
			_createInfos.format,									// format.
			_createInfos.extent,									// extent.

			1,														// mipLevels.
			1,														// arrayLayers.
			VK_SAMPLE_COUNT_1_BIT,									// samples.
			VK_IMAGE_TILING_OPTIMAL,								// tiling.

			_createInfos.usage,										// usage.

			VK_SHARING_MODE_EXCLUSIVE,								// sharingMode.
			VkQueueFamilyIndices::familyNum,						// queueFamilyIndexCount.
			_device.GetQueueFamilyIndices().GetFamilies(),			// pQueueFamilyIndices.
			VK_IMAGE_LAYOUT_UNDEFINED,								// initialLayout.
		};

		SA_VK_ASSERT(vkCreateImage(_device, &imageCreateInfo, nullptr, &mImage),
			CreationFailed, Rendering, L"Failed to create image!");


		// Create Image memory.
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(_device, mImage, &memRequirements);

		uint32 memoryTypeIndex = VkBuffer::FindMemoryType(_device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		const VkMemoryAllocateInfo memoryAllocInfo
		{
			VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,					// sType.
			nullptr,												// pNext.
			memRequirements.size,									// allocationSize.
			memoryTypeIndex,										// memoryTypeIndex.
		};

		SA_VK_ASSERT(vkAllocateMemory(_device, &memoryAllocInfo, nullptr, &mImageMemory),
			MemoryAllocationFailed, Rendering, L"Failed to allocate image memory!");

		vkBindImageMemory(_device, mImage, mImageMemory, 0);



		// Create image view.
		const VkImageViewCreateInfo imageViewCreateInfo
		{
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,									// sType.
			nullptr,																	// pNext.
			VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT,					// flags.
			mImage,																		// image.
			VK_IMAGE_VIEW_TYPE_2D,														// viewType.
			_createInfos.format,														// format.

			VkComponentMapping															// components.
			{
				VK_COMPONENT_SWIZZLE_IDENTITY,										// r.
				VK_COMPONENT_SWIZZLE_IDENTITY,										// g.
				VK_COMPONENT_SWIZZLE_IDENTITY,										// b.
				VK_COMPONENT_SWIZZLE_IDENTITY										// a.
			},
			VkImageSubresourceRange														// subresourceRange.
			{
				_createInfos.aspectFlags,											// aspectMask.
				0,																	// baseMipLevel.
				1,																	// levelCount.
				0,																	// baseArrayLayer.
				1																	// layerCount.
			}
		};

		SA_VK_ASSERT(vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &mImageView),
			CreationFailed, Rendering, L"Failed to create image view!");
	}

	void VkImageBuffer::Destroy(const VkDevice& _device)
	{
		vkDestroyImageView(_device, mImageView, nullptr);
		mImageView = VK_NULL_HANDLE;

		vkDestroyImage(_device, mImage, nullptr);
		mImage = VK_NULL_HANDLE;

		vkFreeMemory(_device, mImageMemory, nullptr) ;// Free memory after destroying image: memory no more used.
		mImageMemory = VK_NULL_HANDLE;
	}

	void VkImageBuffer::TransitionImageLayout(const Sa::VkDevice& _device, VkImageLayout _oldLayout, VkImageLayout _newLayout)
	{
		Sa::VkCommandBuffer commandBuffer = VkCommandBuffer::BeginSingleTimeCommands(_device, _device.GetGraphicsQueue());


		VkImageMemoryBarrier barrier
		{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,							// sType.
			nullptr,														// pNext.
			0 /* Set later. */,												// srcAccessMask.
			0 /* Set later. */,												// dstAccessMask.
			_oldLayout,														// oldLayout.
			_newLayout,														// newLayout.
			VK_QUEUE_FAMILY_IGNORED,										// srcQueueFamilyIndex.
			VK_QUEUE_FAMILY_IGNORED,										// dstQueueFamilyIndex.
			mImage,															// image.

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

		// Set stages and access mask.
		if (_oldLayout == VK_IMAGE_LAYOUT_UNDEFINED)
		{
			barrier.srcAccessMask = 0;
			srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;

			if (_newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (_newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

				dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			}
		}
		else if (_oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			if (_newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
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

		VkCommandBuffer::EndSingleTimeCommands(_device, commandBuffer, _device.GetGraphicsQueue());
	}

	void VkImageBuffer::CopyBufferToImage(const VkDevice& _device, VkBuffer _buffer, const VkExtent3D& _extent)
	{
		Sa::VkCommandBuffer commandBuffer = VkCommandBuffer::BeginSingleTimeCommands(_device, _device.GetTransferQueue());


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

			_extent												// imageExtent.
		};

		vkCmdCopyBufferToImage(commandBuffer, _buffer, mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferImageCopy);


		VkCommandBuffer::EndSingleTimeCommands(_device, commandBuffer, _device.GetTransferQueue());
	}


	VkImageBuffer::operator VkImage() const
	{
		return mImage;
	}
	
	VkImageBuffer::operator VkImageView() const
	{
		return mImageView;
	}
	
	VkImageBuffer::operator VkDeviceMemory() const
	{
		return mImageMemory;
	}
}
