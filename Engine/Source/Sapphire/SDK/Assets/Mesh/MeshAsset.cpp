// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Mesh/MeshAsset.hpp>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <Maths/Misc/Maths.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	MeshAsset::MeshAsset(AssetManager& _manager) noexcept : IAsset(_manager, AssetType::Mesh)
	{
	}

	MeshAsset::MeshAsset(AssetManager& _manager, RawT&& _raw) :
		IAsset(_manager, AssetType::Mesh),
		mRawData{ Move(_raw) }
	{
	}

	MeshAsset::MeshAsset(MeshAsset&& _other) noexcept :
		IAsset(Move(_other)),
		mRawData{ Move(_other.mRawData) }
	{
		_other.UnLoad(false);
	}

	MeshAsset::~MeshAsset()
	{
		UnLoad_Internal(true);
	}

	IMesh* MeshAsset::GetResource() const
	{
		IMesh* result = nullptr;

		result = mManager.meshMgr.Query(mFilePath);

		if (result)
			return result;

		return mManager.meshMgr.Load(*this);
	}

	bool MeshAsset::IsValid() const noexcept
	{
		return mRawData.vertices.size() && mRawData.indices.size();
	}

	bool MeshAsset::Load_Internal(std::istringstream&& _hStream, std::fstream& _fStream)
	{
		uint32 vSize = 0u;
		uint32 iSize = 0u;

		uint32 vertexComps;

		// Header.
		if (!(_hStream >> vSize >> iSize >> vertexComps))
		{
			SA_LOG("Can't parse header!", Warning, SDK_Asset);
			return false;
		}

		mRawData.SetLayout(static_cast<VertexComp>(vertexComps));

		mRawData.vertices.resize(vSize);
		mRawData.indices.resize(iSize / sizeof(uint32));

		_fStream.read(reinterpret_cast<char*>(mRawData.vertices.data()), vSize);
		_fStream.read(reinterpret_cast<char*>(mRawData.indices.data()), iSize);

		return true;
	}

	void MeshAsset::UnLoad_Internal(bool _bFreeResources)
	{
		mRawData.vertices.clear();
		mRawData.indices.clear();
	}


	void MeshAsset::Save_Internal(std::fstream& _fStream) const
	{
		SA_ASSERT(IsValid(), Nullptr, SDK_Asset, L"Save nullptr mesh asset!");

		uint32 vSize = BitSizeOf(mRawData.vertices);
		uint32 iSize = BitSizeOf(mRawData.indices);

		// Header.
		_fStream << vSize << ' ' << iSize << ' ' << static_cast<uint32>(mRawData.GetLayout()->comps) << '\n';

		_fStream.write(mRawData.vertices.data(), vSize);
		_fStream.write(reinterpret_cast<const char*>(mRawData.indices.data()), iSize);
	}

	IMesh* MeshAsset::Create(const IRenderInstance& _instance) const
	{
		return IMesh::CreateInstance(_instance, Move(mRawData));
	}


	MeshAsset& MeshAsset::operator=(MeshAsset&& _rhs)
	{
		mRawData = Move(_rhs.mRawData);

		_rhs.UnLoad(false);

		return *this;
	}
}
