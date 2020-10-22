// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffer/VkImageBuffer.hpp>

#include <Core/Algorithms/SizeOf.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/VkDevice.hpp>

#include <Rendering/Vulkan/Buffer/VkBuffer.hpp>
#include <Rendering/Vulkan/Buffer/VkCommandBuffer.hpp>

namespace Sa
{
	VkImageBuffer VkImageBuffer::CreateColorBuffer(const VkDevice& _device, const ImageExtent& _extent,
														const VkFormat _format, SampleBits _sampleBits)
	{
		VkImageBuffer imageBuffer{};

		VkImageBufferCreateInfos colorBufferCreateInfos{};
		colorBufferCreateInfos.format		= _format;
		colorBufferCreateInfos.extent		= _extent;
		colorBufferCreateInfos.usage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;
		colorBufferCreateInfos.mipMapLevels = 1;
		colorBufferCreateInfos.sampleCount	= static_cast<VkSampleCountFlagBits>(_sampleBits);
		colorBufferCreateInfos.aspectFlags	= VK_IMAGE_ASPECT_COLOR_BIT;

		imageBuffer.Create(_device, colorBufferCreateInfos);
		return imageBuffer;
	}

	VkImageBuffer VkImageBuffer::CreateDepthBuffer(const VkDevice& _device, const ImageExtent& _extent,
														SampleBits _sampleBits)
	{
		VkImageBuffer imageBuffer{};

		VkImageBufferCreateInfos imageBufferCreateInfos{};
		imageBufferCreateInfos.format		= VK_FORMAT_D32_SFLOAT;
		imageBufferCreateInfos.extent		= _extent;
		imageBufferCreateInfos.usage		= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		imageBufferCreateInfos.sampleCount	= static_cast<VkSampleCountFlagBits>(_sampleBits);
		imageBufferCreateInfos.aspectFlags	= VK_IMAGE_ASPECT_DEPTH_BIT;

		imageBuffer.Create(_device, imageBufferCreateInfos);

		// TODO Aurel: Don't think it's necessary
		VkTransitionImageInfos depthTransitionInfos{};
		depthTransitionInfos.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthTransitionInfos.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		depthTransitionInfos.aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;

		imageBuffer.TransitionImageLayout(_device, depthTransitionInfos);

		return imageBuffer;
	}

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
			_createInfos.imageFlags,								// flags.
			_createInfos.imageType,									// imageType.
			_createInfos.format,									// format.
			_createInfos.extent,									// extent.

			_createInfos.mipMapLevels,								// mipLevels.
			_createInfos.layerNum,									// arrayLayers.
			_createInfos.sampleCount,								// samples.
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
			0,																			// flags.
			mImage,																		// image.
			_createInfos.imageViewType,													// viewType.
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
				_createInfos.mipMapLevels,											// levelCount.
				0,																	// baseArrayLayer.
				_createInfos.layerNum												// layerCount.
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

	void VkImageBuffer::TransitionImageLayout(const Sa::VkDevice& _device, const VkTransitionImageInfos& _infos)
	{
		Sa::CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_device, _device.GetGraphicsQueue());


		VkImageMemoryBarrier barrier
		{
			VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,							// sType.
			nullptr,														// pNext.
			0 /* Set later. */,												// srcAccessMask.
			0 /* Set later. */,												// dstAccessMask.
			_infos.oldLayout,												// oldLayout.
			_infos.newLayout,												// newLayout.
			VK_QUEUE_FAMILY_IGNORED,										// srcQueueFamilyIndex.
			VK_QUEUE_FAMILY_IGNORED,										// dstQueueFamilyIndex.
			mImage,															// image.

			VkImageSubresourceRange											// subresourceRange.
			{
				_infos.aspectFlags,								// aspectMask.
				0,												// baseMipLevel.
				_infos.mipLevels,								// levelCount.
				0,												// baseArrayLayer.
				_infos.layerCount,								// layerCount.
			}
		};

		VkPipelineStageFlags srcStage = 0;
		VkPipelineStageFlags dstStage = 0;

		// Set stages and access mask.
		if (_infos.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED)
		{
			barrier.srcAccessMask = 0;
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

		CommandBuffer::EndSingleTimeCommands(_device, commandBuffer, _device.GetGraphicsQueue());
	}

	void VkImageBuffer::CopyBufferToImage(const VkDevice& _device, VkBuffer _buffer, VkExtent3D _extent, uint32 _channel, uint32 _mipLevels, uint32 _layerCount)
	{
		Sa::CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_device, _device.GetTransferQueue());

		uint64 offset = 0u;

		std::vector<VkBufferImageCopy> bufferImageCopies;

		for (uint32 currMipLevel = 0u; currMipLevel < _mipLevels; ++currMipLevel)
		{
			bufferImageCopies.emplace_back(VkBufferImageCopy{
				offset,												// bufferOffset.
				0,													// bufferRowLength.
				0,													// bufferImageHeight.

				VkImageSubresourceLayers							// imageSubresource.
				{
					VK_IMAGE_ASPECT_COLOR_BIT,					// aspectMask.
					currMipLevel,								// mipLevel.
					0,											// baseArrayLayer.
					_layerCount,									// layerCount.
				},

				{0, 0, 0},											// imageOffset.

				_extent												// imageExtent.
			});

			offset += _extent.width * _extent.height * _channel;

			if (_extent.width > 1)
				_extent.width >>= 1;

			if (_extent.height > 1)
				_extent.height >>= 1;
		}

		vkCmdCopyBufferToImage(commandBuffer, _buffer, mImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, SizeOf(bufferImageCopies), bufferImageCopies.data());


		CommandBuffer::EndSingleTimeCommands(_device, commandBuffer, _device.GetTransferQueue());
	}


	void VkImageBuffer::GenerateMipmaps(const Sa::VkDevice& _device, VkFormat format, uint32 _width, uint32 _height, uint32 _mipLevels, uint32 _layerCount)
	{
#if SA_DEBUG
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(_device, format, &formatProperties);

		// Image format supports linear blitting.
		SA_ASSERT(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT,
			NotSupported, Rendering, L"Texture image format does not support linear blitting!");
#endif

		Sa::CommandBuffer commandBuffer = CommandBuffer::BeginSingleTimeCommands(_device, _device.GetGraphicsQueue());

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

		CommandBuffer::EndSingleTimeCommands(_device, commandBuffer, _device.GetGraphicsQueue());
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
