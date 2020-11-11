// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkImageBuffer.hpp>

#include <Rendering/Vulkan/System/VkMacro.hpp>
#include <Rendering/Vulkan/System/Device/VkDevice.hpp>

#include <Rendering/Vulkan/Buffers/VkBuffer.hpp>

namespace Sa::Vk
{
	bool ImageBuffer::IsValid() const noexcept
	{
		return mImage != VK_NULL_HANDLE && mImageView != VK_NULL_HANDLE && mImageMemory != VK_NULL_HANDLE;
	}


	void ImageBuffer::Create(const Device& _device, const ImageBufferCreateInfos& _infos)
	{
		// Create Image.
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.pNext = nullptr;
		imageCreateInfo.flags = _infos.imageFlags;
		imageCreateInfo.imageType = _infos.imageType;
		imageCreateInfo.format = _infos.format;
		imageCreateInfo.extent = _infos.extent;
		imageCreateInfo.mipLevels = _infos.mipLevels;
		imageCreateInfo.arrayLayers = _infos.layerNum;
		imageCreateInfo.samples = _infos.sampleCount;
		imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCreateInfo.usage = _infos.usage;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageCreateInfo.queueFamilyIndexCount = 0u;
		imageCreateInfo.pQueueFamilyIndices = nullptr;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		SA_VK_ASSERT(vkCreateImage(_device, &imageCreateInfo, nullptr, &mImage),
			CreationFailed, Rendering, L"Failed to create image!");


		// Create Image memory.
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(_device, mImage, &memRequirements);

		uint32 memoryTypeIndex = Buffer::FindMemoryType(_device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

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

		CreateFromImage(_device, _infos, mImage);
	}
	
	void ImageBuffer::CreateFromImage(const Device& _device, const ImageBufferCreateInfos& _infos, VkImage& _image)
	{
		mImage = _image;

		// Create image view.
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.pNext = nullptr;
		imageViewCreateInfo.flags = 0;
		imageViewCreateInfo.image = mImage;
		imageViewCreateInfo.viewType = _infos.imageViewType;
		imageViewCreateInfo.format = _infos.format;
		imageViewCreateInfo.components = VkComponentMapping {
			VK_COMPONENT_SWIZZLE_IDENTITY,										// r.
			VK_COMPONENT_SWIZZLE_IDENTITY,										// g.
			VK_COMPONENT_SWIZZLE_IDENTITY,										// b.
			VK_COMPONENT_SWIZZLE_IDENTITY										// a.
		};
		imageViewCreateInfo.subresourceRange = VkImageSubresourceRange {
			_infos.aspectFlags,													// aspectMask.
			0,																	// baseMipLevel.
			_infos.mipLevels,													// levelCount.
			0,																	// baseArrayLayer.
			_infos.layerNum														// layerCount.
		};

		SA_VK_ASSERT(vkCreateImageView(_device, &imageViewCreateInfo, nullptr, &mImageView),
			CreationFailed, Rendering, L"Failed to create image view!");
	}
	
	void ImageBuffer::Destroy(const Device& _device)
	{
		vkDestroyImageView(_device, mImageView, nullptr);
		mImageView = VK_NULL_HANDLE;

		// In case, the image was given to the buffer
		if (mImageMemory != VK_NULL_HANDLE)
		{
			vkDestroyImage(_device, mImage, nullptr);
			mImage = VK_NULL_HANDLE;

			// Free memory after destroying image: memory no more used.
			vkFreeMemory(_device, mImageMemory, nullptr);
			mImageMemory = VK_NULL_HANDLE;
		}
	}


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
