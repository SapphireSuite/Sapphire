// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Window/IWindow.hpp>

namespace Sa
{
	const RenderSurface* IWindow::GetRenderSurface() const
	{
		return mRenderSurface;
	}
}
