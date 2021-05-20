// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Assets/Mesh/MeshAsset.hpp>

#include <fstream>

#include <SDK/Wrappers/TinyOBJWrapper.hpp>

namespace Sa
{
	MeshAsset::MeshAsset() noexcept : IAsset(AssetType::Mesh)
	{
	}

	void MeshAsset::SetRawData(RawMesh&& _rawData)
	{
		UnLoad_Internal();

		mRawData = Move(_rawData);
	}

	const RawMesh& MeshAsset::GetRawData() const noexcept
	{
		return mRawData;
	}

	bool MeshAsset::IsValid() const noexcept
	{
		return !mRawData.vertices.empty() && !mRawData.indices.empty();
	}


	bool MeshAsset::Load_Internal(const std::string& _filePath, std::fstream& _fStream)
	{
		// Layout.
		VertexComp layout = VertexComp::None;
		_fStream.read(reinterpret_cast<char*>(&layout), sizeof(VertexComp));
		mRawData.SetLayout(layout);


		// Vertices.
		uint32 vSize = 0u;
		_fStream.read(reinterpret_cast<char*>(&vSize), sizeof(uint32));

		SA_ASSERT(vSize != 0u, InvalidParam, SDK_Asset, L"Parsing error! Empty vertices.");

		mRawData.vertices.resize(vSize);
		_fStream.read(mRawData.vertices.data(), vSize);


		// Indices.
		uint32 iSize = 0u;
		_fStream.read(reinterpret_cast<char*>(&iSize), sizeof(uint32));

		SA_ASSERT(vSize != 0u, InvalidParam, SDK_Asset, L"Parsing error! Empty vertices.");

		mRawData.indices.resize(iSize / sizeof(uint32));
		_fStream.read(reinterpret_cast<char*>(mRawData.indices.data()), iSize);

		return true;
	}

	void MeshAsset::UnLoad_Internal()
	{
		mRawData.vertices.clear();
		mRawData.indices.clear();
	}

	void MeshAsset::Save_Internal(std::fstream& _fStream) const
	{
		// Layout.
		_fStream.write(reinterpret_cast<const char*>(&mRawData.GetLayout()->comps), sizeof(VertexComp));


		// Vertices.
		uint32 vSize = BitSizeOf(mRawData.vertices);
		_fStream.write(reinterpret_cast<const char*>(&vSize), sizeof(uint32));
		_fStream.write(mRawData.vertices.data(), vSize);


		// Indices.
		uint32 iSize = BitSizeOf(mRawData.indices);
		_fStream.write(reinterpret_cast<const char*>(&iSize), sizeof(uint32));
		_fStream.write(reinterpret_cast<const char*>(mRawData.indices.data()), iSize);
	}

	bool MeshAsset::Import(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		return TinyOBJWrapper::ImportOBJ(_resourcePath, *this, _importInfos.As<ImportT>());
	}
}
