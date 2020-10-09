// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <Rendering/Framework/Primitives/Material/RenderMaterialCreateInfos.hpp>

namespace Sa
{
	PipelineCreateInfos RenderMaterialCreateInfos::GeneratePipelineCreateInfos(uint32 _renderPassIndex) const
	{
		return PipelineCreateInfos
		{
			*renderPasses[_renderPassIndex],
			cameras, bDynamicViewport,
			vertexShader, fragementShader,
			textures, matConstants, renderInfos
		};
	}
}