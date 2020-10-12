// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_RENDER_MATERIAL_ASSET_GUARD
#define SAPPHIRE_SDK_RENDER_MATERIAL_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>
#include <SDK/Assets/RenderMaterial/RenderMaterialAssetInfos.hpp>

#include <Rendering/Framework/Primitives/Material/IRenderMaterial.hpp>

namespace Sa
{
	class IRenderSurface;

	class RenderMaterialAsset : public IAsset
	{
	protected:
		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

		void Save_Internal(std::fstream& _fStream) const override;

		static std::vector<RenderMaterialAsset> ImportMTL(IResourceMgrBase& _manager, const std::string& _resourcePath);

	public:
		using ResT = IRenderMaterial;
		using RawT = RenderMaterialRawData;

		RenderMaterialRawData infos;

		SA_ENGINE_API RenderMaterialAsset(IResourceMgrBase& _manager) noexcept;
		SA_ENGINE_API RenderMaterialAsset(RenderMaterialAsset&& _other) noexcept;
		SA_ENGINE_API RenderMaterialAsset(IResourceMgrBase& _manager, RawT&& _raw) noexcept;
		SA_ENGINE_API ~RenderMaterialAsset();

		SA_ENGINE_API ResT* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override;
		SA_ENGINE_API std::vector<AssetPathDependency> GetPathDependencies() const noexcept override final;


		SA_ENGINE_API ResT* Create(const IRenderInstance& _instance) const;


		RenderMaterialAsset& operator=(RenderMaterialAsset&& _rhs);
		RenderMaterialAsset& operator=(const RenderMaterialAsset&) = delete;
	};
}

#endif // GUARD
