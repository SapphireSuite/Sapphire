// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MATERIAL_ASSET_GUARD
#define SAPPHIRE_SDK_MATERIAL_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>

#include <Rendering/Framework/Primitives/Pipeline/AlphaModel.hpp>
#include <Rendering/Framework/Primitives/Pipeline/PolygonMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/CullingMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/FrontFaceMode.hpp>
#include <Rendering/Framework/Primitives/Pipeline/IlluminationModel.hpp>

#include <Rendering/Framework/Primitives/Material/UniformBuffers.hpp>
#include <Rendering/Framework/Primitives/Material/MaterialConstants.hpp>

namespace Sa
{
	class IRenderSurface;

	class MaterialAsset : public IAsset
	{
	protected:
		void Save_Internal(std::fstream& _fStream) const override;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

		static std::vector<MaterialAsset> ImportMTL(const std::string& _resourcePath);

	public:
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


		SA_ENGINE_API MaterialAsset() noexcept;
		SA_ENGINE_API ~MaterialAsset();


		SA_ENGINE_API IRenderMaterial* Create(const IRenderInstance& _instance, const IRenderSurface& _surface);

		SA_ENGINE_API static std::vector<MaterialAsset> Import(const std::string& _resourcePath);


		SA_ENGINE_API MaterialAsset& operator=(MaterialAsset&& _rhs);
		MaterialAsset& operator=(const MaterialAsset&) = delete;
	};
}

#endif // GUARD
