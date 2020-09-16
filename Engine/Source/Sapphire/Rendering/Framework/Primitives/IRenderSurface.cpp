// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/IRenderSurface.hpp>

namespace Sa
{
	void IRenderSurface::ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height)
	{
		(void)_width;
		(void)_height;

		onResizeEvent(_instance, *this);
	}
}
