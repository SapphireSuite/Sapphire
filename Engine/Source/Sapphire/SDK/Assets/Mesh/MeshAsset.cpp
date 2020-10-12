// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Mesh/MeshAsset.hpp>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <Maths/Misc/Maths.hpp>

#include <SDK/Assets/AssetManager.hpp>

namespace Sa
{
	MeshAsset::MeshAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::Mesh)
	{
	}

	MeshAsset::MeshAsset(IResourceMgrBase& _manager, RawT&& _raw) :
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
		auto& manager = mManager.As<ResourceMgr<MeshAsset>>();

		result = manager.Query(mFilePath);

		if (result)
			return result;

		return manager.Load(*this);
	}

	bool MeshAsset::IsValid() const noexcept
	{
		return mRawData.vertices.size() && mRawData.indices.size();
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

		mRawData.vertices.resize(vSize / sizeof(Vertex));
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
		_fStream << vSize << ' ' << iSize << '\n';

		_fStream.write(reinterpret_cast<const char*>(mRawData.vertices.data()), vSize);
		_fStream.write(reinterpret_cast<const char*>(mRawData.indices.data()), iSize);
	}

	IMesh* MeshAsset::Create(const IRenderInstance& _instance) const
	{
		return IMesh::CreateInstance(_instance, Move(mRawData));
	}


	void MeshAsset::ComputeTangent()
	{
		for (uint32 i = 0; i + 3 < SizeOf(mRawData.indices); i += 3)
		{
			Vec3f edge1 = mRawData.vertices[mRawData.indices[i + 1]].position - mRawData.vertices[mRawData.indices[i]].position;
			Vec3f edge2 = mRawData.vertices[mRawData.indices[i + 2]].position - mRawData.vertices[mRawData.indices[i]].position;

			Vec2f deltaUV1 = mRawData.vertices[mRawData.indices[i + 1]].texture - mRawData.vertices[mRawData.indices[i]].texture;
			Vec2f deltaUV2 = mRawData.vertices[mRawData.indices[i + 2]].texture - mRawData.vertices[mRawData.indices[i]].texture;

			Vec3f tangent;
			float ratio = deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y;

			if (Maths::Equals0(ratio))
				tangent = Vec3f::Zero;
			else
			{
				float f = 1.0f / ratio ;

				tangent = (f * Vec3f(deltaUV2.y * edge1.x - deltaUV1.y * edge2.x,
					deltaUV2.y * edge1.y - deltaUV1.y * edge2.y,
					deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)).Normalize();
			}

			mRawData.vertices[mRawData.indices[i]].tangent =
				mRawData.vertices[mRawData.indices[i + 1]].tangent =
				mRawData.vertices[mRawData.indices[i + 2]].tangent = tangent;
		}
	}


	MeshAsset& MeshAsset::operator=(MeshAsset&& _rhs)
	{
		mRawData = Move(_rhs.mRawData);

		_rhs.UnLoad(false);

		return *this;
	}
}
