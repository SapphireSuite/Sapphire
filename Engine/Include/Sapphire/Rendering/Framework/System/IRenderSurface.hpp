// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD
#define SAPPHIRE_RENDERING_RENDER_SURFACE_GUARD

#include <Core/Types/IInterface.hpp>

namespace Sa
{
	class IRenderInstance;

	class IRenderSurface : public IInterface
	{
	public:
		virtual void Create(const IRenderInstance& _instance) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
