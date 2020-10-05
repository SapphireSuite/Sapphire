// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_ASSET_GUARD
#define SAPPHIRE_SDK_MESH_ASSET_GUARD

#include <SDK/Resources/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

namespace Sa
{
	class MeshAsset : public IAsset
	{
	protected:
		std::vector<Vertex> mVertices;
		std::vector<uint32> mIndices;

		void Save_Internal(std::fstream& _fStream) const override;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override;
		void UnLoad_Internal(bool _bFreeResources) override;

		SA_ENGINE_API static std::vector<MeshAsset> ImportOBJ(const std::string& _resourcePath);

	public:
		SA_ENGINE_API MeshAsset() noexcept;
		SA_ENGINE_API MeshAsset(std::vector<Vertex>&& _vertices, std::vector<uint32>&& _indices);
		SA_ENGINE_API MeshAsset(const std::vector<Vertex>& _vertices, const std::vector<uint32>& _indices);
		SA_ENGINE_API ~MeshAsset();

		SA_ENGINE_API IMesh* Create(const IRenderInstance& _instance);

		SA_ENGINE_API static std::vector<MeshAsset> Import(const std::string& _resourcePath);


		SA_ENGINE_API MeshAsset& operator=(MeshAsset&& _rhs);
		MeshAsset& operator=(const MeshAsset&) = delete;
	};
}

#endif // GUARD
