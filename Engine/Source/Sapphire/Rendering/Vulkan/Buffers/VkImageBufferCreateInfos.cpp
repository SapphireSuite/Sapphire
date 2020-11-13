// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Vulkan/Buffers/VkImageBufferCreateInfos.hpp>

namespace Sa::Vk
{
	VkImageCreateInfo ImageBufferCreateInfos::VkImageInfos() const noexcept
	{
		VkImageCreateInfo vkInfos{};
		vkInfos.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vkInfos.pNext = nullptr;

		vkInfos.flags = imageFlags;
		vkInfos.imageType = API_GetImageType(imageType);
		
		if (imageType == ImageType::Cube)
			vkInfos.flags |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		
		vkInfos.format = API_GetFormat(format);
		vkInfos.extent = VkExtent3D{ extent.x, extent.y, 1 };
		vkInfos.mipLevels = mipLevels;

		vkInfos.arrayLayers = API_GetLayerNum(imageType);
		
		vkInfos.samples = static_cast<VkSampleCountFlagBits>(sampling);
		vkInfos.tiling = VK_IMAGE_TILING_OPTIMAL;
		
		vkInfos.usage = usage;


		if (IsColorFormat(format))
			vkInfos.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		else if (IsDepthFormat(format))
			vkInfos.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

		vkInfos.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkInfos.queueFamilyIndexCount = 0u;
		vkInfos.pQueueFamilyIndices = nullptr;
		vkInfos.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		return vkInfos;
	}

	VkImageViewCreateInfo ImageBufferCreateInfos::VkImageViewInfos(VkImage _image) const noexcept
	{
		VkImageViewCreateInfo vkInfos{};

		vkInfos.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vkInfos.pNext = nullptr;
		vkInfos.flags = 0;
		vkInfos.image = _image;
		vkInfos.viewType = API_GetImageViewType(imageType);
		vkInfos.format = API_GetFormat(format);
		
		vkInfos.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkInfos.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkInfos.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkInfos.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		
		VkImageAspectFlags vkAspectFlags = aspectFlags;

		if (IsColorFormat(format))
				vkAspectFlags |= VK_IMAGE_ASPECT_COLOR_BIT;
		else if (IsDepthFormat(format))
			vkAspectFlags |= VK_IMAGE_ASPECT_DEPTH_BIT;


		vkInfos.subresourceRange.aspectMask = vkAspectFlags;
		vkInfos.subresourceRange.baseMipLevel = 0u;
		vkInfos.subresourceRange.levelCount = mipLevels;
		vkInfos.subresourceRange.baseArrayLayer = 0u;
		vkInfos.subresourceRange.layerCount = API_GetLayerNum(imageType);

		return vkInfos;
	}
}
