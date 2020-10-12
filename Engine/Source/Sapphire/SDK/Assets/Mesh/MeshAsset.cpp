// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Mesh/MeshAsset.hpp>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	MeshAsset::MeshAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::Mesh)
	{
	}

	MeshAsset::MeshAsset(IResourceMgrBase& _manager, RawDataT&& _rawData) :
		IAsset(_manager, AssetType::Mesh),
		mVertices{ Move(_rawData.vertices) },
		mIndices{ Move(_rawData.indices) }
	{
	}

	MeshAsset::MeshAsset(MeshAsset&& _other) noexcept : IAsset(Move(_other))
	{
		mVertices = Move(_other.mVertices);
		mIndices = Move(_other.mIndices);

		_other.UnLoad(false);
	}

	MeshAsset::~MeshAsset()
	{
		UnLoad_Internal(true);
	}

	IMesh* MeshAsset::GetResource() const
	{
		IMesh* result = nullptr;
		auto& manager = mManager.As<ResourceMgr<MeshAsset>>();

		result = manager.Query(mFilePath);

		if (result)
			return result;

		return manager.Load(*this);
	}

	bool MeshAsset::IsValid() const noexcept
	{
		return mVertices.size() && mIndices.size();
	}

	bool MeshAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		uint32 vSize = 0u;
		uint32 iSize = 0u;

		// Header.
		if (!(_hStream >> vSize >> iSize))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		mVertices.resize(vSize / sizeof(Vertex));
		mIndices.resize(iSize / sizeof(uint32));

		_fStream.read(reinterpret_cast<char*>(mVertices.data()), vSize);
		_fStream.read(reinterpret_cast<char*>(mIndices.data()), iSize);

		return true;
	}

	void MeshAsset::UnLoad_Internal(bool _bFreeResources)
	{
		mVertices.clear();
		mIndices.clear();
	}


	void MeshAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(!mVertices.empty() && !mIndices.empty(), Nullptr, SDK_Asset, L"Save nullptr texture asset!");

		uint32 vSize = SizeOf(mVertices) * sizeof(Vertex);
		uint32 iSize = SizeOf(mIndices) * sizeof(uint32);

		// Header.
		_fStream << vSize << ' ' << iSize << '\n';

		_fStream.write(reinterpret_cast<const char*>(mVertices.data()), vSize);
		_fStream.write(reinterpret_cast<const char*>(mIndices.data()), iSize);
	}

	IMesh* MeshAsset::Create(const IRenderInstance& _instance) const
	{
		return IMesh::CreateInstance(_instance, mVertices, mIndices);
	}


	MeshAsset& MeshAsset::operator=(MeshAsset&& _rhs)
	{
		mVertices = Move(_rhs.mVertices);
		mIndices = Move(_rhs.mIndices);

		_rhs.UnLoad(false);

		return *this;
	}
}
