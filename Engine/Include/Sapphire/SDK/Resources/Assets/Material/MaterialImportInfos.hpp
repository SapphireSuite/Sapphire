// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MATERIAL_IMPORT_INFOS_GUARD
#define SAPPHIRE_SDK_MATERIAL_IMPORT_INFOS_GUARD

#include <SDK/Resources/Assets/IAssetImportInfos.hpp>

#include <Rendering/Framework/Primitives/Pipeline/AlphaModel.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PolygonMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/CullingMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/FrontFaceMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/IlluminationModel.hpp>

#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>
#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>

namespace Sa
{
	struct MaterialImportInfos : public IAssetImportInfos
	{
		std::vector<std::string> outFilePaths;

		SA_ENGINE_API MaterialImportInfos() = default;
		SA_ENGINE_API MaterialImportInfos(std::vector<std::string> && _outFilePaths, bool _bKeepLoaded = true) noexcept;
		SA_ENGINE_API MaterialImportInfos(const std::vector<std::string> & _outFilePaths, bool _bKeepLoaded = true) noexcept;
	};

	struct MaterialCreateInfos : public IAssetCreateInfos
	{
		std::string vertexShaderPath;
		std::string fragmentShaderPath;

		std::vector<std::string> texturePaths;

		MaterialConstants matConstants;

		uint32 uniformBufferSize = sizeof(ObjectUniformBuffer);

		AlphaModel alphaModel = AlphaModel::Opaque;
		PolygonMode polygonMode = PolygonMode::Fill;
		CullingMode cullingMode = CullingMode::Back;
		FrontFaceMode frontFaceMode = FrontFaceMode::Clockwise;
		IlluminationModel illumModel = IlluminationModel::PBR;
	};
}

#endif // GUARD
