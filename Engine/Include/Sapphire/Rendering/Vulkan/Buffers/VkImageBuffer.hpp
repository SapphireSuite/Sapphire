// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa::Vk
{
	class Device;

	struct ImageBufferCreateInfos;

	class ImageBuffer
	{
		VkImage mImage = VK_NULL_HANDLE;
		VkImageView mImageView = VK_NULL_HANDLE;
		VkDeviceMemory mImageMemory = VK_NULL_HANDLE;

	public:
		bool IsValid() const noexcept;


		void Create(const Device& _device, const ImageBufferCreateInfos& _infos);
		void CreateFromImage(const Device& _device, const ImageBufferCreateInfos& _infos, VkImage& _image);
		void Destroy(const Device& _device);


		operator VkImage() const noexcept;
		operator VkImageView() const noexcept;
		operator VkDeviceMemory() const noexcept;
	};

	struct ImageBufferCreateInfos
	{
		uint32 imageFlags = 0u;
		VkImageType imageType = VK_IMAGE_TYPE_2D;
		VkFormat format = VK_FORMAT_UNDEFINED;
		VkExtent3D extent = { 0, 0, 0 };
		uint32 mipLevels = 1u;
		uint32 layerNum = 1u;
		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
		VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
		uint32 usage = 0u;

		VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;
		VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	};
}

#endif

#endif // GUARD
