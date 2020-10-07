// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_ASSET_GUARD
#define SAPPHIRE_SDK_MESH_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>
#include <SDK/Resources/Assets/Mesh/MeshAssetInfos.hpp>

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

namespace Sa
{
	class MeshAsset : public IAsset
	{
	protected:
		std::vector<Vertex> mVertices;
		std::vector<uint32> mIndices;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override final;
		void UnLoad_Internal(bool _bFreeResources) override final;

		void Save_Internal(std::fstream& _fStream) const override final;
		void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) override final;

		static std::vector<MeshAsset> ImportOBJ(IResourceMgrBase& _manager, const std::string& _resourcePath);

	public:
		using ImportInfosT = MeshImportInfos;
		using ImportResultT = std::vector<MeshAsset>;
		using CreateInfosT = MeshCreateInfos;

		SA_ENGINE_API MeshAsset(IResourceMgrBase& _manager) noexcept;
		SA_ENGINE_API MeshAsset(IResourceMgrBase& _manager, MeshCreateInfos&& _createInfos);
		SA_ENGINE_API MeshAsset(MeshAsset&& _other) noexcept;
		SA_ENGINE_API ~MeshAsset();

		SA_ENGINE_API IMesh* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override final;

		SA_ENGINE_API IMesh* Create(const IRenderInstance& _instance) const;

		SA_ENGINE_API static std::vector<MeshAsset> Import(IResourceMgrBase& _manager, const std::string& _resourcePath, const MeshImportInfos& _importInfos);


		MeshAsset& operator=(MeshAsset&& _rhs);
		MeshAsset& operator=(const MeshAsset&) = delete;
	};
}

#endif // GUARD
