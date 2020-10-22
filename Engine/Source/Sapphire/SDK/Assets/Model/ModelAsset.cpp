// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Model/ModelAsset.hpp>

#include <Core/Algorithms/Move.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	ModelAsset::ModelAsset(AssetManager& _manager) : IAsset(_manager, AssetType::Model)
	{
	}

	void ModelAsset::Add(RawMesh&& _mesh)
	{
		meshes.emplace_back(mManager, Move(_mesh));
	}

	void ModelAsset::Add(RawMaterial&& _mat)
	{
		renderMats.emplace_back(mManager, Move(_mat));
	}


	// TODO: IMPLEMENTATION.
	bool ModelAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		return false;
	}

	void ModelAsset::UnLoad_Internal(bool _bFreeResources)
	{
	}

	void ModelAsset::Save_Internal(std::fstream& _fStream) const
	{
	}

	bool ModelAsset::IsValid() const noexcept
	{
		return false;
	}
}