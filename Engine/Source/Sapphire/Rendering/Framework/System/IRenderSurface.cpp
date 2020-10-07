// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/System/IRenderSurface.hpp>

namespace Sa
{
	// TODO: REMOVE LATER.
	IRenderSurface* IRenderSurface::TEMP = nullptr;


	void IRenderSurface::ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height)
	{
		(void)_width;
		(void)_height;

		//onResizeEvent(_instance, *this);
	}
}
