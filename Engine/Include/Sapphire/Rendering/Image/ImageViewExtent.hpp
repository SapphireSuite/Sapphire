// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IMAGE_VIEW_EXTENT_GUARD
#define SAPPHIRE_RENDERING_IMAGE_VIEW_EXTENT_GUARD

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Image/ImageExtent.hpp>

namespace Sa
{
	class ImageViewExtent
	{
	public:
		ImageExtent extent;

		Vec2<uint32> offset;

		ImageViewExtent() = default;
		SA_ENGINE_API ImageViewExtent(const ImageExtent& _extent, const Vec2<uint32>& _offset = Vec2<uint32>::Zero) noexcept;

		bool operator==(const ImageViewExtent& _rhs) const noexcept;
		bool operator!=(const ImageViewExtent& _rhs) const noexcept;
	};
}

#endif // GUARD
