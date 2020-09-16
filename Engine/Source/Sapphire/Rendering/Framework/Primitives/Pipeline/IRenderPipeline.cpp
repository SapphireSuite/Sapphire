// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Pipeline/IRenderPipeline.hpp>
#include <Rendering/Framework/Primitives/IRenderSurface.hpp>

namespace Sa
{
	void IRenderPipeline::Create(const IRenderInstance& _instance, const IRenderSurface& _surface, const std::vector<const IShader*>& _shaders)
	{
		Create(_instance, _surface, _shaders, _surface.GetViewport());
	}
}