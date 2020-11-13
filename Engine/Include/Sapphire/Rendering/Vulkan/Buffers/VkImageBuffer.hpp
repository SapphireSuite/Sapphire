// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD
#define SAPPHIRE_RENDERING_VK_IMAGE_BUFFER_GUARD

#include <Rendering/Vulkan/Buffers/VkImageBufferCreateInfos.hpp>

#if SA_RENDERING_API == SA_VULKAN

namespace Sa::Vk
{
	class Device;

	class SA_ENGINE_API ImageBuffer
	{
		VkImage mImage = VK_NULL_HANDLE;
		VkImageView mImageView = VK_NULL_HANDLE;
		VkDeviceMemory mImageMemory = VK_NULL_HANDLE;

	public:
		struct TransitionInfos
		{
			VkImageLayout oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			VkImageLayout newLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			uint32 mipLevels = 1u;
			ImageType imageType = ImageType::Image2D;

			VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
		};

		struct CopyBufferImageInfos
		{
			VkBuffer buffer = VK_NULL_HANDLE;
			
			Vec2ui extent;
			Format format = Format::RGBA_32;

			uint32 mipLevels = 1u;
			ImageType imageType = ImageType::Image2D;
		};

		bool IsValid() const noexcept;

		void CreateImage(const Device& _device, const VkImageCreateInfo& _vkInfos);
		void CreateImageView(const Device& _device, const VkImageViewCreateInfo& _vkInfos);

		void Create(const Device& _device, const ImageBufferCreateInfos& _infos);
		void CreateFromImage(const Device& _device, const ImageBufferCreateInfos& _infos, VkImage& _image);
		
		void Destroy(const Device& _device);

		void TransitionImageLayout(const Device& _device, const ImageBuffer::TransitionInfos& _infos);
		void CopyBufferToImage(const Device& _device, const ImageBuffer::CopyBufferImageInfos& _infos);

		VkDescriptorImageInfo CreateDescriptorImageInfo(VkSampler _sampler = VK_NULL_HANDLE) const noexcept;

		// Old mipmap generation. Mipmap are now generated on resource import (See StbiWrapper).
		//void GenerateMipmaps(const Device& _device, VkFormat format, uint32 _width, uint32 _height, uint32 _mipLevels, uint32 _layerNum = 1u);


		operator VkImage() const noexcept;
		operator VkImageView() const noexcept;
		operator VkDeviceMemory() const noexcept;
	};
}

#endif

#endif // GUARD
