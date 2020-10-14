// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_RENDERING_PIPELINE_CREATE_INFOS_GUARD
#define SAPPHIRE_RENDERING_PIPELINE_CREATE_INFOS_GUARD

#include <Rendering/Config.hpp>

#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PipelineRenderInfos.hpp>

namespace Sa
{
	class IShader;
	class ITexture;
	class IRenderPass;

	class ICamera;

	struct PipelineCreateInfos
	{
		const IRenderPass& renderPass;
		
		const std::vector<const ICamera*>& cameras;
		bool bDynamicViewport = true;

		const IShader* vertexShader = nullptr;
		const IShader* fragementShader = nullptr;

		const std::vector<const ITexture*>& textures;

		const MaterialConstants& matConstants;

		const PipelineRenderInfos& renderInfos;
	};
}

#endif // GUARD
