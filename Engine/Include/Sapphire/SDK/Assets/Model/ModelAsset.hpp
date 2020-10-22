// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MODEL_ASSET_GUARD
#define SAPPHIRE_SDK_MODEL_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <SDK/Assets/Mesh/MeshAsset.hpp>
#include <SDK/Assets/RenderMaterial/RenderMaterialAsset.hpp>

namespace Sa
{
	class ModelAsset : public IAsset
	{
	protected:
		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override final;
		void UnLoad_Internal(bool _bFreeResources) override final;

		void Save_Internal(std::fstream& _fStream) const override final;

	public:
		std::vector<MeshAsset> meshes;
		std::vector<RenderMaterialAsset> renderMats;

		ModelAsset(AssetManager& _manager);

		void Add(RawMesh&& _mesh);
		void Add(RawMaterial&& _mat);

		SA_ENGINE_API bool IsValid() const noexcept override final;
	};
}

#endif // GUARD
