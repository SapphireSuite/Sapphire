// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MATERIAL_ASSET_GUARD
#define SAPPHIRE_SDK_MATERIAL_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>
#include <SDK/Resources/Assets/Material/MaterialAssetInfos.hpp>

#include <Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>

namespace Sa
{
	class IRenderSurface;

	class MaterialAsset : public IAsset
	{
	protected:
		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

		void Save_Internal(std::fstream& _fStream) const override;
		void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) override;

		static std::vector<MaterialAsset> ImportMTL(IResourceMgrBase& _manager, const std::string& _resourcePath);

	public:
		using ImportInfosT = MaterialImportInfos;
		using ImportResultT = std::vector<MaterialAsset>;
		using CreateInfosT = MaterialCreateInfos;

		MaterialCreateInfos infos;

		SA_ENGINE_API MaterialAsset(IResourceMgrBase& _manager) noexcept;
		SA_ENGINE_API MaterialAsset(MaterialAsset&& _other) noexcept;
		SA_ENGINE_API MaterialAsset(IResourceMgrBase& _manager, MaterialCreateInfos&& _infos) noexcept;
		SA_ENGINE_API ~MaterialAsset();

		SA_ENGINE_API IRenderMaterial* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override;
		SA_ENGINE_API std::vector<AssetPathDependency> GetPathDependencies() const noexcept override final;


		SA_ENGINE_API IRenderMaterial* Create(const IRenderInstance& _instance) const;

		SA_ENGINE_API static std::vector<MaterialAsset> Import(IResourceMgrBase& _manager,
			const std::string& _resourcePath, const MaterialImportInfos& _importInfos);


		MaterialAsset& operator=(MaterialAsset&& _rhs);
		MaterialAsset& operator=(const MaterialAsset&) = delete;
	};
}

#endif // GUARD
