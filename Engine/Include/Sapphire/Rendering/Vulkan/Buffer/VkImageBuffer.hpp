// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD

#include <Core/Types/Int.hpp>

#include <Rendering/Config.hpp>

#include <Rendering/Image/ImageExtent.hpp>
#include <Rendering/Framework/System/RenderPass/SampleBits.hpp>

#if SA_RENDERING_API == SA_VULKAN

#include <vulkan/vulkan.h>

namespace Sa
{
	class VkDevice;
	class VkBuffer;

	struct VkImageBufferCreateInfos
	{
		VkFormat format = VK_FORMAT_UNDEFINED;

		VkExtent3D extent = {0, 0, 0};
		
		uint32 usage = 0u;

		uint32 mipMapLevels = 1u;

		VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;

		VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

		uint32 layerNum = 1u;

		VkImageType imageType = VK_IMAGE_TYPE_2D;

		VkImageViewType imageViewType = VK_IMAGE_VIEW_TYPE_2D;

		uint32 imageFlags = 0u;
	};

	struct VkTransitionImageInfos
	{
		VkImageLayout oldLayout;
		VkImageLayout newLayout;

		uint32 mipLevels = 1u;
		uint32 layerCount = 1u;

		VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
	};

	class VkImageBuffer
	{
		VkImage mImage = VK_NULL_HANDLE;
		VkImageView mImageView = VK_NULL_HANDLE;
		VkDeviceMemory mImageMemory = VK_NULL_HANDLE;

	public:
		// Copy of 3 pointers so it's 'okay'
		static VkImageBuffer CreateColorBuffer(const VkDevice& _device, const ImageExtent& _extent,
									const VkFormat _format, const SampleBits _sampleBits = SampleBits::Sample1Bit);
		static VkImageBuffer CreateDepthBuffer(const VkDevice& _device, const ImageExtent& _extent,
									const SampleBits _sampleBits = SampleBits::Sample1Bit);

	public:
		bool IsValid();

		void Create(const VkDevice& _device, const VkImageBufferCreateInfos& _createInfos);
		void CreateFromImage(const VkDevice& _device, const VkImageBufferCreateInfos& _createInfos, VkImage& _image);
		void Destroy(const VkDevice& _device);

		void TransitionImageLayout(const VkDevice& _device, const VkTransitionImageInfos& _infos);
		void CopyBufferToImage(const VkDevice& _device, VkBuffer _buffer, VkExtent3D _extent, uint32 _channel, uint32 _mipLevels = 1u, uint32 _layerCount = 1u);

		// Old mipmap generation. Mipmap are now generated on resource import (See StbiWrapper).
		void GenerateMipmaps(const Sa::VkDevice& _device, VkFormat format, uint32 _width, uint32 _height, uint32 _mipLevels, uint32 _layerNum = 1u);

		operator VkImage() const;
		operator VkImageView() const;
		operator VkDeviceMemory() const;
	};
}

#endif

#endif // GUARD
