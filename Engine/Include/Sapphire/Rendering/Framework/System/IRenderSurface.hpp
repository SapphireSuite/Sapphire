// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD

#include <Core/Types/IInterface.hpp>

#include <Core/Types/Variadics/Event.hpp>

#include <Rendering/Image/Viewport.hpp>

namespace Sa
{
	class IRenderInstance;

	class IRenderSurface : public IInterface
	{
	public:
		// TODO: REMOVE LATER.
		static IRenderSurface* TEMP;

		//mutable Event<void(const IRenderInstance&, const IRenderSurface&)> onResizeEvent;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height);

		virtual const ImageExtent& GetImageExtent() const noexcept = 0;
		virtual Viewport GetViewport() const noexcept  = 0;
	};
}

#endif // GUARD
