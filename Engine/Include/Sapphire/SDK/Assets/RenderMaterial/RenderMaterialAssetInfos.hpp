// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_RENDER_MATERIAL_ASSET_INFOS_GUARD
#define SAPPHIRE_SDK_RENDER_MATERIAL_ASSET_INFOS_GUARD

#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PipelineRenderInfos.hpp>

namespace Sa
{
	class IRenderPass;
	class Camera;

	struct RenderMaterialRawData
	{
		const IRenderPass* renderPass = nullptr;

		std::vector<const Camera*> cameras;
		bool bDynamicViewport = true;

		std::string vertexShaderPath;
		std::string fragmentShaderPath;

		std::vector<std::string> texturePaths;

		MaterialConstants matConstants;
		PipelineRenderInfos renderInfos;
	};
}

#endif // GUARD
