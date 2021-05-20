// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_ASSET_GUARD
#define SAPPHIRE_SDK_MESH_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

#include <SDK/Assets/Mesh/MeshImportInfos.hpp>

namespace Sa
{
	class SA_ENGINE_API MeshAsset : public IAsset
	{
	protected:
		RawMesh mRawData;

		bool Load_Internal(const std::string& _filePath, std::fstream& _fStream) override final;
		void UnLoad_Internal() override final;

		void Save_Internal(std::fstream& _fStream) const override final;

	public:
		using ImportT = MeshImportInfos;

		MeshAsset() noexcept;

		void SetRawData(RawMesh&& _rawData);
		const RawMesh& GetRawData() const noexcept;

		bool IsValid() const noexcept override final;

		bool Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos = ImportT()) override final;
	};
}

#endif // GUARD
