// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_RENDER_MATERIAL_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_RENDER_MATERIAL_CREATE_INFOS_GUARD

#include <vector>

#include <Rendering/Framework/Primitives/Pipeline/PipelineCreateInfos.hpp>

namespace Sa
{
	class IRenderPass;
	class Camera;

	struct RenderMaterialCreateInfos
	{
		IRenderPass& renderPass;

		std::vector<const Camera*> cameras;
		bool bDynamicViewport = true;

		const IShader* vertexShader = nullptr;
		const IShader* fragementShader = nullptr;

		std::vector<const ITexture*> textures;
	
		MaterialConstants matConstants;
		PipelineRenderInfos renderInfos;

		SA_ENGINE_API PipelineCreateInfos GeneratePipelineCreateInfos(/*uint32 _renderPassIndex*/) const;
	};
}

#endif // GUARD
