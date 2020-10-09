// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Image/ImageViewExtent.hpp>

namespace Sa
{
	ImageViewExtent::ImageViewExtent(const ImageExtent& _extent, const Vec2<uint32>& _offset) noexcept :
		extent{ _extent },
		offset{ _offset }
	{
	}

	bool ImageViewExtent::operator==(const ImageViewExtent& _rhs) const noexcept
	{
		return offset == _rhs.offset && extent == _rhs.extent;
	}

	bool ImageViewExtent::operator!=(const ImageViewExtent& _rhs) const noexcept
	{
		return !(*this == _rhs);
	}
}
