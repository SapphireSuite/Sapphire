// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_INSTANCE_GUARD
#define SAPPHIRE_RENDERING_IRENDER_INSTANCE_GUARD

#include <Rendering/Config.hpp>

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	class IWindow;
	class IRenderSurface;

	class IRenderInstance : public IInterface
	{
	public:
		virtual void Create() = 0;
		virtual void Destroy() = 0;

		virtual const IRenderSurface& CreateRenderSurface(const IWindow& _window) = 0;
		virtual void DestroyRenderSurface(const IWindow& _window) = 0;

		virtual void Update() = 0;
	};
}

#endif // GUARD
