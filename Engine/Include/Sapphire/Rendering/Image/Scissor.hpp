// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_SCISSOR_GUARD
#define SAPPHIRE_RENDERING_SCISSOR_GUARD

#include <Maths/Space/Vector2.hpp>

#include <Rendering/Image/ImageExtent.hpp>

namespace Sa
{
	class Scissor
	{
	public:
		Vec2<uint32> offset;

		ImageExtent extent;
	};
}

#endif // GUARD
