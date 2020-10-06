// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MATERIAL_ASSET_GUARD
#define SAPPHIRE_SDK_MATERIAL_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>
#include <SDK/Resources/Assets/Material/MaterialImportInfos.hpp>

#include <Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>

namespace Sa
{
	class IRenderSurface;

	class MaterialAsset : public IAsset
	{
	protected:
		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

		void Save_Internal(std::fstream& _fStream, const std::string& _newPath) const override;
		void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) override;

		static std::vector<MaterialAsset> ImportMTL(AssetManager& _manager, const std::string& _resourcePath);

	public:
		using ImportInfoClass = MaterialImportInfos;

		MaterialCreateInfos infos;

		MaterialAsset(AssetManager& _manager) noexcept;
		MaterialAsset(MaterialAsset&& _other) noexcept;
		MaterialAsset(AssetManager& _manager, MaterialCreateInfos&& _infos) noexcept;
		SA_ENGINE_API ~MaterialAsset();

		SA_ENGINE_API IRenderMaterial* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override;


		IRenderMaterial* Create(const IRenderInstance& _instance, const IRenderSurface& _surface) const;

		static std::vector<MaterialAsset> Import(AssetManager& _manager, const std::string& _resourcePath, const MaterialImportInfos& _importInfos);


		MaterialAsset& operator=(MaterialAsset&& _rhs);
		MaterialAsset& operator=(const MaterialAsset&) = delete;
	};
}

#endif // GUARD
