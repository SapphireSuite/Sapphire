// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_IRENDER_PIPELINE_GUARD

#include <Rendering/Image/Viewport.hpp>

namespace Sa
{
	class IRenderInstance;
	class IRenderSurface;
	class Shader;

	class IRenderPipeline
	{
	public:
		void Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const std::vector<Shader>& _shaders);
		virtual void Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const std::vector<Shader>& _shaders, const std::vector<Viewport>& _viewports) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
