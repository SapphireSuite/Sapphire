// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_VIEWPORT_GUARD
#define SAPPHIRE_RENDERING_VIEWPORT_GUARD

#include <Rendering/Image/Scissor.hpp>

namespace Sa
{
	class ViewPort
	{
	public:
		Vec2<uint32> offset;

		ImageExtent extent;

		Scissor scissor;
	};
}

#endif // GUARD
