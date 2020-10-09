// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MATERIAL_ASSET_INFOS_GUARD
#define SAPPHIRE_SDK_MATERIAL_ASSET_INFOS_GUARD

#include <SDK/Resources/Assets/IAssetInfos.hpp>

#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PipelineRenderInfos.hpp>

namespace Sa
{
	class IRenderPass;
	class Camera;

	struct MaterialImportInfos : public IAssetImportInfos
	{
		std::vector<std::string> outFilePaths;

		SA_ENGINE_API MaterialImportInfos() = default;
		SA_ENGINE_API MaterialImportInfos(std::vector<std::string> && _outFilePaths, bool _bKeepLoaded = true) noexcept;
		SA_ENGINE_API MaterialImportInfos(const std::vector<std::string> & _outFilePaths, bool _bKeepLoaded = true) noexcept;
	};

	struct MaterialCreateInfos : public IAssetCreateInfos
	{
		std::vector<const IRenderPass*> renderPasses;

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
