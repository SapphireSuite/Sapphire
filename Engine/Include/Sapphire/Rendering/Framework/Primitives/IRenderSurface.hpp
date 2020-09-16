// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD

#include <Rendering/Image/Viewport.hpp>

namespace Sa
{
	class IRenderSurface
	{
	public:
		virtual const ImageExtent& GetImageExtent() const noexcept = 0;
		virtual Viewport GetViewport() const noexcept  = 0;
	};
}

#endif // GUARD
