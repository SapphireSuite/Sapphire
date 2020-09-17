// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_IRENDER_PIPELINE_GUARD
#define SAPPHIRE_RENDERING_IRENDER_PIPELINE_GUARD

#include <vector>

#include <Core/Types/IInterface.hpp>

#include <Rendering/Image/Viewport.hpp>

namespace Sa
{
	class IRenderInstance;
	class IRenderSurface;
	class IShader;

	class IRenderPipeline : public IInterface
	{
	public:
		void Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const std::vector<const IShader*>& _shaders);
		virtual void Create(const IRenderInstance& _instance,
			const IRenderSurface& _surface,
			const std::vector<const IShader*>& _shaders,
			const Viewport& _viewport) = 0;
		virtual void Destroy(const IRenderInstance& _instance) = 0;
	};
}

#endif // GUARD
