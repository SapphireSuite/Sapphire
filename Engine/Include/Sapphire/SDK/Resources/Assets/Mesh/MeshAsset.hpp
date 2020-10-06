// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_ASSET_GUARD
#define SAPPHIRE_SDK_MESH_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>
#include <SDK/Resources/Assets/Mesh/MeshImportInfos.hpp>

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

namespace Sa
{
	class MeshAsset : public IAsset
	{
	protected:
		std::vector<Vertex> mVertices;
		std::vector<uint32> mIndices;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

		void Save_Internal(std::fstream& _fStream, const std::string& _newPath) const override;
		void Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos) override;

		static std::vector<MeshAsset> ImportOBJ(AssetManager& _manager, const std::string& _resourcePath);

	public:
		using ImportInfoClass = MeshImportInfos;

		MeshAsset(AssetManager& _manager) noexcept;
		MeshAsset(AssetManager& _manager, std::vector<Vertex>&& _vertices, std::vector<uint32>&& _indices);
		MeshAsset(AssetManager& _manager, const std::vector<Vertex>& _vertices, const std::vector<uint32>& _indices);
		MeshAsset(MeshAsset&& _other) noexcept;
		SA_ENGINE_API ~MeshAsset();

		SA_ENGINE_API IMesh* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override;

		IMesh* Create(const IRenderInstance& _instance) const;

		static std::vector<MeshAsset> Import(AssetManager& _manager, const std::string& _resourcePath, const MeshImportInfos& _importInfos);


		MeshAsset& operator=(MeshAsset&& _rhs);
		MeshAsset& operator=(const MeshAsset&) = delete;
	};
}

#endif // GUARD
