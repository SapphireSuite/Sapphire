// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Misc/ImageType.hpp>

#include <Collections/Debug>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	VkImageType API_GetImageType(ImageType _type)
	{
		switch (_type)
		{
			case Sa::ImageType::Image2D:
			case Sa::ImageType::Cube:
				return VK_IMAGE_TYPE_2D;
			case Sa::ImageType::Image3D:
				return VK_IMAGE_TYPE_3D;
			default:
				SA_LOG("ImageType not supported yet!", Warning, Rendering);
				return VK_IMAGE_TYPE_2D;
		}
	}

	VkImageViewType API_GetImageViewType(ImageType _type)
	{
		switch (_type)
		{
			case Sa::ImageType::Image2D:
				return VK_IMAGE_VIEW_TYPE_2D;
			case Sa::ImageType::Cube:
				return VK_IMAGE_VIEW_TYPE_CUBE;
			case Sa::ImageType::Image3D:
				return VK_IMAGE_VIEW_TYPE_3D;
			default:
				SA_LOG("ImageViewType not supported yet!", Warning, Rendering);
				return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	uint32 API_GetLayerNum(ImageType _type)
	{
		switch (_type)
		{
			case Sa::ImageType::Image2D:
				return 1u;
			case Sa::ImageType::Image3D:
				return 1u;
			case Sa::ImageType::Cube:
				return 6u;
			default:
				SA_LOG("ImageType not supported yet!", Warning, Rendering);
				return 1u;
		}
	}

#endif
}
