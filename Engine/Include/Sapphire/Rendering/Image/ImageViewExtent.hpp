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
		Vec2<uint32> offset;

		ImageExtent extent;
	};
}

#endif // GUARD