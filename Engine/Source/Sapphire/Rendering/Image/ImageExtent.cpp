// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Image/ImageExtent.hpp>

namespace Sa
{
#if SA_RENDERING_API == SA_VULKAN

	ImageExtent::ImageExtent(const VkExtent2D& _extent) :
		width{_extent.width },
		height{ _extent.height }
	{
	}

	ImageExtent::operator const VkExtent2D&() const
	{
		return reinterpret_cast<const VkExtent2D&>(*this);
	}

#endif
}
