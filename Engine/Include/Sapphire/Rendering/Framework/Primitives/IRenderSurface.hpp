// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD

#include <Rendering/Image/ImageExtent.hpp>

namespace Sa
{
	class IRenderSurface
	{
	public:
		virtual const ImageExtent& GetImageExtent() const = 0;
	};
}

#endif // GUARD
