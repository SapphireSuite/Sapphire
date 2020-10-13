// Copyright 2020 Sapphire development team. All Rights Reserved.

#pragma once

#ifndef SAPPHIRE_SDK_MESH_ASSET_GUARD
#define SAPPHIRE_SDK_MESH_ASSET_GUARD

#include <SDK/Assets/IAsset.hpp>

#include <Rendering/Framework/Primitives/Mesh/IMesh.hpp>

namespace Sa
{
	class MeshAsset : public IAsset
	{
	protected:
		RawMesh mRawData;

		bool Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream) override final;
		void UnLoad_Internal(bool _bFreeResources) override final;

		void Save_Internal(std::fstream& _fStream) const override final;

		static std::vector<MeshAsset> ImportOBJ(IResourceMgrBase& _manager, const std::string& _resourcePath);
		static std::vector<MeshAsset> ImportGLTF(IResourceMgrBase& _manager, const std::string& _resourcePath);

	public:
		using ResT = IMesh;
		using RawT = RawMesh;

		SA_ENGINE_API MeshAsset(IResourceMgrBase& _manager) noexcept;
		SA_ENGINE_API MeshAsset(IResourceMgrBase& _manager, RawT&& _rawData);
		SA_ENGINE_API MeshAsset(MeshAsset&& _other) noexcept;
		SA_ENGINE_API ~MeshAsset();

		SA_ENGINE_API IMesh* GetResource() const;
		SA_ENGINE_API bool IsValid() const noexcept override final;

		SA_ENGINE_API IMesh* Create(const IRenderInstance& _instance) const;

		SA_ENGINE_API void ComputeTangents();

		MeshAsset& operator=(MeshAsset&& _rhs);
		MeshAsset& operator=(const MeshAsset&) = delete;
	};
}

#endif // GUARD
