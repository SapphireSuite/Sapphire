// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD

#include <Core/Types/Int.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkDevice;
	class VkBuffer;

	struct VkImageBufferCreateInfos
	{
		VkFormat format;

		VkExtent3D extent;
		
		uint32 usage = 0u;

		uint32 mipMapLevels = 1u;

		VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	};

	class VkImageBuffer
	{
		VkImage mImage = VK_NULL_HANDLE;
		VkImageView mImageView = VK_NULL_HANDLE;
		VkDeviceMemory mImageMemory = VK_NULL_HANDLE;

	public:
		bool IsValid();

		void Create(const VkDevice& _device, const VkImageBufferCreateInfos& _createInfos);
		void Destroy(const VkDevice& _device);

		void TransitionImageLayout(const VkDevice& _device, VkImageLayout _oldLayout, VkImageLayout _newLayout, uint32 _mipLevels = 1u);
		void CopyBufferToImage(const VkDevice& _device, VkBuffer _buffer, const VkExtent3D& _extent);

		void GenerateMipmaps(const Sa::VkDevice& _device, VkFormat format, uint32 _width, uint32 _height, uint32 _mipLevels);

		operator VkImage() const;
		operator VkImageView() const;
		operator VkDeviceMemory() const;
	};
}

#endif

#endif // GUARD
