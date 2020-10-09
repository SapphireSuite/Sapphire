// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD

#include <Core/Types/IInterface.hpp>

#include <Core/Types/Variadics/Event.hpp>

namespace Sa
{
	class ImageExtent;

	class IRenderInstance;
	class IRenderPass;
	struct RenderPassCreateInfos;

	class IRenderSurface : public IInterface
	{
	public:
		//mutable Event<void(const IRenderInstance&, const IRenderSurface&)> onResizeEvent;

		// TODO: Remove SA_ENGINE_API.
		SA_ENGINE_API virtual void ResizeCallback(const IRenderInstance& _instance, uint32 _width, uint32 _height);

		virtual const ImageExtent& GetImageExtent() const noexcept = 0;

		virtual IRenderPass& CreateRenderPass(const IRenderInstance& _instance, const RenderPassCreateInfos& _createInfos) = 0;
		virtual void DestroyRenderPass(const IRenderInstance& _instance, IRenderPass& _renderPass) = 0;
	};
}

#endif // GUARD
