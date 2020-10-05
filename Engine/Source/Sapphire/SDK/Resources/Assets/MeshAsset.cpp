// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <SDK/Resources/Assets/MeshAsset.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <Core/Algorithms/Move.hpp>
#include <Core/Types/Variadics/SizeOf.hpp>

namespace Sa
{
#if SA_DEBUG

	// Supported extensions.
	constexpr const char* extensions[]{ "obj" };

#endif

	MeshAsset::MeshAsset() noexcept : IAsset(AssetType::Mesh)
	{
	}

	MeshAsset::MeshAsset(std::vector<Vertex>&& _vertices, std::vector<uint32>&& _indices) :
		IAsset(AssetType::Mesh),
		mVertices{ Move(_vertices) },
		mIndices{ Move(_indices) }
	{
	}

	MeshAsset::MeshAsset(const std::vector<Vertex>& _vertices, const std::vector<uint32>& _indices) :
		IAsset(AssetType::Mesh),
		mVertices{ _vertices },
		mIndices{ _indices }
	{
	}

	MeshAsset::~MeshAsset()
	{
		UnLoad_Internal(true);
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

	IMesh* MeshAsset::Create(const IRenderInstance& _instance)
	{
		return IMesh::CreateInstance(_instance, mVertices, mIndices);
	}

	std::vector<MeshAsset> MeshAsset::Import(const std::string& _resourcePath)
	{
		SA_ASSERT(!CheckExtensionSupport(_resourcePath, extensions, SizeOf(extensions)),
			WrongExtension, SDK_Import, L"Mesh file extension not supported yet!");

		std::string extension = GetResourceExtension(_resourcePath);

		if (extension == "obj")
			return ImportOBJ(_resourcePath);


		SA_ASSERT(false, WrongExtension, SDK_Import, L"Mesh file extension not supported yet!");

		return std::vector<MeshAsset>();
	}

	std::vector<MeshAsset> MeshAsset::ImportOBJ(const std::string& _resourcePath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::string warn; std::string error;

		SA_ASSERT(tinyobj::LoadObj(&attrib, &shapes, nullptr, &warn, &error, _resourcePath.c_str()),
			InvalidParam, Rendering, L"Failed to load obj model!");

		std::vector<MeshAsset> meshes(shapes.size());

		for (uint32 i = 0u; i < shapes.size(); ++i)
		{
			std::vector<Vertex>& vertices = meshes[i].mVertices;
			std::vector<uint32>& indices = meshes[i].mIndices;

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
