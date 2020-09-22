// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Image/ImageExtent.hpp>

namespace Sa
{
	ImageExtent::ImageExtent(uint32 _width, uint32 _height) :
		width{ _width },
		height{ _height }
	{
	}

#if SA_RENDERING_API == SA_VULKAN

	ImageExtent::ImageExtent(const VkExtent2D& _extent) :
		width{_extent.width },
		height{ _extent.height }
	{
	}

	ImageExtent::operator VkExtent2D() const
	{
		return VkExtent2D{ width , height };
	}

	ImageExtent::operator VkExtent3D() const
	{
		return VkExtent3D{ width , height, 1 };
	}

#endif
}
