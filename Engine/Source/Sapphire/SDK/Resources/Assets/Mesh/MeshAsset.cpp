// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/Mesh/MeshAsset.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <Core/Algorithms/Move.hpp>
#include <Core/Algorithms/SizeOf.hpp>

#include <SDK/Resources/Assets/AssetManager.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "obj" };

#endif

	MeshAsset::MeshAsset(IResourceMgrBase& _manager) noexcept : IAsset(_manager, AssetType::Mesh)
	{
	}

	MeshAsset::MeshAsset(IResourceMgrBase& _manager, MeshCreateInfos&& _createInfos) :
		IAsset(_manager, AssetType::Mesh, Move(_createInfos)),
		mVertices{ Move(_createInfos.vertices) },
		mIndices{ Move(_createInfos.indices) }
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
		return mManager.As<ResourceMgr<IMesh, MeshAsset>>().Query(mFilePath);
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

	void MeshAsset::Import_Internal(const std::string& _resourcePath, const IAssetImportInfos& _importInfos)
	{
		IAsset::Import_Internal(_resourcePath, _importInfos);
	}


	IMesh* MeshAsset::Create(const IRenderInstance& _instance) const
	{
		return IMesh::CreateInstance(_instance, mVertices, mIndices);
	}

	std::vector<MeshAsset> MeshAsset::Import(IResourceMgrBase& _manager, const std::string& _resourcePath, const MeshImportInfos& _importInfos)
	{
		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			WrongExtension, SDK_Import, L"Mesh file extension not supported yet!");

		std::vector<MeshAsset> result;
		std::string extension = GetResourceExtension(_resourcePath);

		if (extension == "obj")
			result = ImportOBJ(_manager, _resourcePath);
		else
			SA_ASSERT(false, WrongExtension, SDK_Import, L"Mesh file extension not supported yet!");


		for (uint32 i = 0u; i < SizeOf(result); ++i)
		{
			result[i].mFilePath = _importInfos.outFilePaths[i];
			result[i].Import_Internal(_resourcePath, _importInfos);
		}

		return result;
	}

	std::vector<MeshAsset> MeshAsset::ImportOBJ(IResourceMgrBase& _manager, const std::string& _resourcePath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::string warn; std::string error;

		SA_ASSERT(tinyobj::LoadObj(&attrib, &shapes, nullptr, &warn, &error, _resourcePath.c_str()),
			InvalidParam, Rendering, L"Failed to load obj model!");

		std::vector<MeshAsset> meshes;
		meshes.reserve(shapes.size());

		for (uint32 i = 0u; i < shapes.size(); ++i)
		{
			MeshAsset& mesh = meshes.emplace_back(_manager);
			std::vector<Vertex>& vertices = mesh.mVertices;
			std::vector<uint32>& indices = mesh.mIndices;

			for (auto indexIt = shapes[i].mesh.indices.begin(); indexIt != shapes[i].mesh.indices.end(); ++indexIt)
			{
				const Vertex vertex
				{
					Vec3f(attrib.vertices[3 * indexIt->vertex_index + 0],
					attrib.vertices[3 * indexIt->vertex_index + 1],
					attrib.vertices[3 * indexIt->vertex_index + 2]),

					Vec3f(attrib.normals[3 * indexIt->normal_index + 0],
					attrib.normals[3 * indexIt->normal_index + 1],
					attrib.normals[3 * indexIt->normal_index + 2]),

					Vec2f(attrib.texcoords[2 * indexIt->texcoord_index + 0],
					attrib.texcoords[2 * indexIt->texcoord_index + 1])
				};

				vertices.push_back(vertex);
				indices.push_back(static_cast<uint32>(indices.size()));
			}
		}

		return meshes;
	}


	MeshAsset& MeshAsset::operator=(MeshAsset&& _rhs)
	{
		mVertices = Move(_rhs.mVertices);
		mIndices = Move(_rhs.mIndices);

		_rhs.UnLoad(false);

		return *this;
	}
}
